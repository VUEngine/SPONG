/* VUEngine - Virtual Utopia Engine <http://vuengine.planetvb.com/>
 * A universal game engine for the Nintendo Virtual Boy
 *
 * Copyright (C) 2007, 2018 by Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <chris@vr32.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Game.h>
#include <KeyPadManager.h>
#include <MessageDispatcher.h>
#include <GameEvents.h>
#include <PongState.h>
#include <debugUtilities.h>
#include <CommunicationManager.h>
#include "Player.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define SCORE_MULTIPLIER_THRESHOLD			5
#define BONUS_INCREMENT_DELAY				100
#define SCORE_MULTIPLIER_TO_ENABLE_BONUS	5


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void Player::constructor()
{
	Base::constructor();

	this->pongBall = NULL;
	this->playerPaddles = new VirtualList();
	this->opponentPaddles = new VirtualList();

	this->leftScore = 0;
	this->rightScore = 0;
	this->totalLeftScore = 0;
	this->totalRightScore = 0;
	this->totalScore = 0;
	this->scoreMultiplier = 1;
	this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
	this->ballIsRolling = false;
}

void Player::destructor()
{
	this->pongBall = NULL;
	delete this->playerPaddles;
	this->playerPaddles = NULL;
	delete this->opponentPaddles;
	this->opponentPaddles = NULL;

	// allow a new construct
	Base::destructor();
}

bool Player::handleMessage(Telegram telegram __attribute__ ((unused)))
{
	switch(Telegram::getMessage(telegram))
	{
		case kAddBonusScore:

			this->leftScore += 1;
			this->rightScore += 1;
			Player::printScore(this);

			MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, Object::safeCast(this), Object::safeCast(this), kAddBonusScore, NULL);
			break;
	}

	return false;
}


void Player::getReady(GameState gameState)
{
	this->playerNumber = kPlayerAlone;

	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		if(CommunicationManager::isMaster(CommunicationManager::getInstance()))
		{
			this->playerNumber = kPlayerOne;
		}
		else
		{
			this->playerNumber = kPlayerTwo;
		}
	}

	if(kPlayerAlone == this->playerNumber)
	{
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(2 == VirtualList::getSize(this->playerPaddles), "Player::getReady: not all paddles found");
	}
	else if(kPlayerOne == this->playerNumber)
	{
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find left paddle");
		VirtualList::pushBack(this->opponentPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find right paddle");
	}
	else if(kPlayerOne == this->playerNumber)
	{
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find right paddle");
		VirtualList::pushBack(this->opponentPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->opponentPaddles), "Player::getReady: didn't find left paddle");
	}

	this->pongBall = PongBall::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PONG_BALL_NAME, true));

	Object::addEventListener(Object::safeCast(gameState), Object::safeCast(this), (EventListener)Player_onUserInput, kEventUserInput);
	Object::addEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::addEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object::addEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);

	Player::printScore(this);
	KeypadManager::registerInput(KeypadManager::getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);
}

void Player::gameIsOver(GameState gameState)
{
	VirtualList::clear(this->playerPaddles);
	VirtualList::clear(this->opponentPaddles);

	Object::removeEventListener(Object::safeCast(gameState), Object::safeCast(this), (EventListener)Player_onUserInput, kEventUserInput);
	Object::removeEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::removeEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object::removeEventListener(Object::safeCast(this->pongBall), Object::safeCast(this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);
}

// process user input
void Player::onUserInput(Object eventFirer __attribute__ ((unused)))
{
	ASSERT(PongState::safeCast(eventFirer), "Player::onUserInput: wrong event firer");

	UserInput userInput = KeypadManager::getUserInput(KeypadManager::getInstance());

	if(userInput.pressedKey)
	{
		Player::onKeyPressed(this, &userInput, this->playerPaddles);
	}

	if(userInput.releasedKey)
	{
		Player::onKeyReleased(this, &userInput, this->playerPaddles);
	}

	if(userInput.holdKey)
	{
		Player::onKeyHold(this, &userInput, this->playerPaddles);
	}
}

void Player::onOpponentInput(UserInput userInput)
{
	if(userInput.pressedKey)
	{
		Player::onKeyPressed(this, &userInput, this->opponentPaddles);
	}

	if(userInput.releasedKey)
	{
		Player::onKeyReleased(this, &userInput, this->opponentPaddles);
	}

	if(userInput.holdKey)
	{
		Player::onKeyHold(this, &userInput, this->opponentPaddles);
	}
}

void Player::onKeyPressed(const UserInput* userInput, VirtualList paddles)
{
	if(K_B & userInput->pressedKey)
	{
	}

	if(K_A & userInput->pressedKey)
	{
	}

	// check direction
	if((K_LL | K_LR ) & (userInput->pressedKey | userInput->holdKey))
	{
	}
	else if(K_LU & userInput->pressedKey)
	{
	}
}

void Player::onKeyReleased(const UserInput* userInput, VirtualList paddles)
{
	if(K_B & userInput->releasedKey)
	{
	}

	if(K_A & userInput->releasedKey)
	{
		Player::ejectPaddles(this, paddles);
	}
/*
	s8 horizontalInput = 0;
	s8 verticalInput = 0;

	// check direction
	if(K_LL & userInput->releasedKey)
	{
		horizontalInput = -1;
	}
	else if(K_LR & userInput->releasedKey)
	{
		horizontalInput = 1;
	}

	if(K_LU & userInput->releasedKey)
	{
		verticalInput = -1;
	}
	else if(K_LD & userInput->releasedKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player::stopPaddles(this, horizontalInput, verticalInput);
	}
*/
}

void Player::onKeyHold(const UserInput* userInput, VirtualList paddles)
{
	if(K_B & userInput->holdKey)
	{
	}

	if(K_A & userInput->holdKey)
	{
		Player::retractPaddles(this, paddles);
	}

	s8 horizontalInput = 0;
	s8 verticalInput = 0;

	// check direction
	if((K_LL) & userInput->holdKey)
	{
		horizontalInput = -1;
	}
	else if((K_LR) & userInput->holdKey)
	{
		horizontalInput = 1;
	}

	if((K_LU) & userInput->holdKey)
	{
		verticalInput = -1;
	}
	else if((K_LD) & userInput->holdKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player::movePaddles(this, paddles, horizontalInput, verticalInput);
	}
}

void Player::movePaddles(VirtualList paddles, s8 horizontalInput, s8 verticalInput)
{
	Direction direction =
	{
		horizontalInput,
		verticalInput,
		0
	};

	VirtualNode node = VirtualList::begin(paddles);

	for(; node; node = VirtualNode::getNext(node))
	{
		Paddle::moveTowards(VirtualNode::getData(node), direction);
	}
}

void Player::stopPaddles(VirtualList paddles, s8 horizontalInput, s8 verticalInput)
{
	Direction direction =
	{
		horizontalInput,
		verticalInput,
		0
	};

	VirtualNode node = VirtualList::begin(paddles);

	for(; node; node = VirtualNode::getNext(node))
	{
		//Paddle::stopTowards(VirtualNode::getData(node), direction);
	}
}

void Player::retractPaddles(VirtualList paddles)
{
	VirtualNode node = VirtualList::begin(paddles);

	for(; node; node = VirtualNode::getNext(node))
	{
		Paddle::retract(VirtualNode::getData(node));
	}
}

void Player::ejectPaddles(VirtualList paddles)
{
	VirtualNode node = VirtualList::begin(paddles);

	for(; node; node = VirtualNode::getNext(node))
	{
		Paddle::eject(VirtualNode::getData(node));
	}
}

void Player::onPongBallHitFloor(Object eventFirer __attribute__ ((unused)))
{
	if(this->scoreMultiplier >= SCORE_MULTIPLIER_TO_ENABLE_BONUS)
	{
		PongBall::startRolling(this->pongBall);
		this->ballIsRolling = true;
		MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, Object::safeCast(this), Object::safeCast(this), kAddBonusScore, NULL);
	}
	else
	{
		switch(PongBall::getPaddleEnum(this->pongBall))
		{
			case kLeftPaddle:

				this->totalLeftScore += this->scoreMultiplier * this->leftScore;
				this->totalRightScore += this->rightScore;
				break;

			case kRightPaddle:

				this->totalLeftScore += this->leftScore;
				this->totalRightScore += this->scoreMultiplier * this->rightScore;
				break;
		}

		Player::totalizeScore(this);
	}
}

void Player::totalizeScore()
{
	this->totalLeftScore += this->scoreMultiplier * this->leftScore;
	this->totalRightScore += this->scoreMultiplier * this->rightScore;
	this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
	this->scoreMultiplier = 1;
	this->leftScore = 0;
	this->rightScore = 0;
	this->totalScore = this->totalLeftScore + this->totalRightScore;

	Player::printScore(this);
}

void Player::onPongBallHitCeiling(Object eventFirer __attribute__ ((unused)))
{
	switch(PongBall::getPaddleEnum(this->pongBall))
	{
		case kLeftPaddle:

			this->leftScore += 5;
			break;

		case kRightPaddle:

			this->rightScore += 5;
			break;
	}

	this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
	this->scoreMultiplier += 2;

	Player::printScore(this);
}

void Player::onPongBallHitPaddle(Object eventFirer __attribute__ ((unused)))
{
	if(this->ballIsRolling)
	{
		this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
		this->scoreMultiplier = 1;

		this->ballIsRolling = false;

		switch(PongBall::getPaddleEnum(this->pongBall))
		{
			case kLeftPaddle:

				this->leftScore = 0;
				break;

			case kRightPaddle:

				this->rightScore = 0;
				break;
		}

		Player::totalizeScore(this);

		MessageDispatcher::discardDelayedMessagesFromSender(MessageDispatcher::getInstance(), Object::safeCast(this), kAddBonusScore);
	}
	else
	{
		switch(PongBall::getPaddleEnum(this->pongBall))
		{
			case kLeftPaddle:

				this->leftScore++;
				break;

			case kRightPaddle:

				this->rightScore++;
				break;
		}

		if(0 >= --this->scoreMultiplierThreshold)
		{
			this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
			this->scoreMultiplier++;
		}

		Player::printScore(this);
	}
}

void Player::printScore()
{
	PRINT_TEXT("Total:      ", 1, 0);
	PRINT_INT(this->totalLeftScore, 10, 0);
	PRINT_TEXT("Current:    ", 1, 1);
	PRINT_INT(this->leftScore, 10, 1);

	PRINT_TEXT("Total:      ", 35, 0);
	PRINT_INT(this->totalRightScore, 35 + 9, 0);
	PRINT_TEXT("Current:    ", 35, 1);
	PRINT_INT(this->rightScore, 35 + 9, 1);

	PRINT_TEXT("TOTAL:     ", 20, 0);
	PRINT_INT(this->totalScore, 20 + 7, 0);
	PRINT_TEXT("(X):       ", 22, 1);
	PRINT_INT(this->scoreMultiplier, 20 + 7, 1);

	PRINT_TEXT("Player ", 20, 27);
	PRINT_INT(this->playerNumber, 28, 27);

}
