/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <VUEngine.h>
#include <KeypadManager.h>
#include <MessageDispatcher.h>
#include <GameEvents.h>
#include <PongState.h>
#include <debugUtilities.h>
#include <CommunicationManager.h>
#include <SoundManager.h>
#include "Player.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define SCORE_MULTIPLIER_THRESHOLD			5
#define BONUS_INCREMENT_DELAY				100
#define SCORE_MULTIPLIER_TO_ENABLE_BONUS	10


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

bool Player::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMessageAddBonusScore:

			this->leftScore += 1;
			this->rightScore += 1;
			//SoundManager::playFxSound(SoundManager::getInstance(), BALL_START_ROLLING_SND, *SpatialObject::getPosition(SpatialObject::safeCast(this->pongBall)));
			Player::printScore(this);

			MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, ListenerObject::safeCast(this), ListenerObject::safeCast(this), kMessageAddBonusScore, NULL);
			break;
	}

	return false;
}


void Player::getReady(GameState gameState)
{
	this->playerNumber = kPlayerAlone;

	if(PongState::getVersusMode(PongState::getInstance()))
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
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(2 == VirtualList::getSize(this->playerPaddles), "Player::getReady: not all paddles found");
	}
	else if(kPlayerOne == this->playerNumber)
	{
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find left paddle");
		VirtualList::pushBack(this->opponentPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find right paddle");
	}
	else if(kPlayerTwo == this->playerNumber)
	{
		VirtualList::pushBack(this->playerPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_RIGHT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->playerPaddles), "Player::getReady: didn't find right paddle");
		VirtualList::pushBack(this->opponentPaddles, Paddle::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PADDLE_LEFT_NAME, true)));
		ASSERT(1 == VirtualList::getSize(this->opponentPaddles), "Player::getReady: didn't find left paddle");
	}

	this->pongBall = PongBall::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), (char*)PONG_BALL_NAME, true));

	ListenerObject::addEventListener(gameState, ListenerObject::safeCast(this), (EventListener)Player::onUserInput, kEventUserInput);
	ListenerObject::addEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitFloor, kEventPongBallHitFloor);
	ListenerObject::addEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitCeiling, kEventPongBallHitCeiling);
	ListenerObject::addEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitPaddle, kEventPongBallHitPaddle);

	Player::printScore(this);
	KeypadManager::registerInput(KeypadManager::getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);
}

void Player::gameIsOver(GameState gameState)
{
	VirtualList::clear(this->playerPaddles);
	VirtualList::clear(this->opponentPaddles);

	ListenerObject::removeEventListener(gameState, ListenerObject::safeCast(this), (EventListener)Player::onUserInput, kEventUserInput);
	ListenerObject::removeEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitFloor, kEventPongBallHitFloor);
	ListenerObject::removeEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitCeiling, kEventPongBallHitCeiling);
	ListenerObject::removeEventListener(this->pongBall, ListenerObject::safeCast(this), (EventListener)Player::onPongBallHitPaddle, kEventPongBallHitPaddle);
}

// process user input
void Player::onUserInput(ListenerObject eventFirer __attribute__ ((unused)))
{
	ASSERT(PongState::safeCast(eventFirer), "Player::onUserInput: wrong event firer");

	UserInput userInput = KeypadManager::getUserInput(KeypadManager::getInstance());

	if(userInput.pressedKey)
	{
		Player::onKeyPressed(this, userInput.pressedKey, this->playerPaddles);
	}

	if(userInput.releasedKey)
	{
		Player::onKeyReleased(this, userInput.releasedKey, this->playerPaddles);
	}

	if(userInput.holdKey)
	{
		Player::onKeyHold(this, userInput.holdKey, this->playerPaddles);
	}

	ResumedUserInput resumedUserInput = PongState::getOpponentInput(PongState::safeCast(eventFirer));

	if(resumedUserInput.pressedKey)
	{
		Player::onKeyPressed(this, resumedUserInput.pressedKey, this->opponentPaddles);
	}

	if(resumedUserInput.releasedKey)
	{
		Player::onKeyReleased(this, resumedUserInput.releasedKey, this->opponentPaddles);
	}

	if(resumedUserInput.holdKey)
	{
		Player::onKeyHold(this, resumedUserInput.holdKey, this->opponentPaddles);
	}
}

void Player::onKeyPressed(uint16 pressedKey, VirtualList paddles __attribute__ ((unused)))
{
	if(K_B & pressedKey)
	{
	}

	if(K_A & pressedKey)
	{
	}
}

void Player::onKeyReleased(uint16 releasedKey, VirtualList paddles)
{
	if(K_B & releasedKey)
	{
	}

	if(K_A & releasedKey)
	{
		Player::ejectPaddles(this, paddles);
	}
/*
	int8 horizontalInput = 0;
	int8 verticalInput = 0;

	// check direction
	if(K_LL & releasedKey)
	{
		horizontalInput = -1;
	}
	else if(K_LR & releasedKey)
	{
		horizontalInput = 1;
	}

	if(K_LU & releasedKey)
	{
		verticalInput = -1;
	}
	else if(K_LD & releasedKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player::stopPaddles(this, horizontalInput, verticalInput);
	}
*/
}

void Player::onKeyHold(uint16 holdKey, VirtualList paddles)
{
	if(K_B & holdKey)
	{
	}

	if(K_A & holdKey)
	{
		Player::retractPaddles(this, paddles);
	}

	int8 horizontalInput = 0;
	int8 verticalInput = 0;

	// check direction
	if((K_LL | K_RL) & holdKey)
	{
		horizontalInput = -1;
	}
	else if((K_LR | K_RR) & holdKey)
	{
		horizontalInput = 1;
	}

	if((K_LU | K_RU) & holdKey)
	{
		verticalInput = -1;
	}
	else if((K_LD | K_RD) & holdKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player::movePaddles(this, paddles, horizontalInput, verticalInput);
	}
}

void Player::movePaddles(VirtualList paddles, int8 horizontalInput, int8 verticalInput)
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

PongBall Player::getPongBall()
{
	return this->pongBall;
}

void Player::stopPaddles(VirtualList paddles, int8 horizontalInput __attribute__ ((unused)), int8 verticalInput __attribute__ ((unused)))
{
	/*Direction direction =
	{
		horizontalInput,
		verticalInput,
		0
	};*/

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

void Player::onPongBallHitFloor(ListenerObject eventFirer __attribute__ ((unused)))
{
	if(this->scoreMultiplier >= SCORE_MULTIPLIER_TO_ENABLE_BONUS)
	{
		PongBall::startRolling(this->pongBall);
		this->ballIsRolling = true;
		MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, ListenerObject::safeCast(this), ListenerObject::safeCast(this), kMessageAddBonusScore, NULL);

		//SoundManager::playFxSound(SoundManager::getInstance(), BALL_START_ROLLING_SND, *SpatialObject::getPosition(SpatialObject::safeCast(eventFirer)));
	}
	else
	{
		Player::totalizeScore(this);

		//SoundManager::playFxSound(SoundManager::getInstance(), BALL_HIT_FLOOR_SND, *SpatialObject::getPosition(SpatialObject::safeCast(eventFirer)));
	}
}

void Player::totalizeScore()
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

	this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
	this->scoreMultiplier = 1;
	this->leftScore = 0;
	this->rightScore = 0;
	this->totalScore = this->totalLeftScore + this->totalRightScore;

	Player::printScore(this);
}

void Player::onPongBallHitCeiling(ListenerObject eventFirer __attribute__ ((unused)))
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

	//SoundManager::playFxSound(SoundManager::getInstance(), BALL_HIT_CEILING_SND, *SpatialObject::getPosition(SpatialObject::safeCast(eventFirer)));
}

void Player::onPongBallHitPaddle(ListenerObject eventFirer __attribute__ ((unused)))
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

		MessageDispatcher::discardDelayedMessagesFromSender(MessageDispatcher::getInstance(), ListenerObject::safeCast(this), kMessageAddBonusScore);
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

	//SoundManager::playFxSound(SoundManager::getInstance(), BALL_HIT_PADDLE_SND, *SpatialObject::getPosition(SpatialObject::safeCast(eventFirer)));
}

int Player::getPlayerNumber()
{
	return this->playerNumber;
}


void Player::printScore()
{
	PRINT_TEXT("Total:	  ", 1, 0);
	PRINT_INT(this->totalLeftScore, 10, 0);
	PRINT_TEXT("Current:	", 1, 1);
	PRINT_INT(this->leftScore, 10, 1);

	PRINT_TEXT("Total:	  ", 35, 0);
	PRINT_INT(this->totalRightScore, 35 + 9, 0);
	PRINT_TEXT("Current:	", 35, 1);
	PRINT_INT(this->rightScore, 35 + 9, 1);

	PRINT_TEXT("TOTAL:	 ", 20, 0);
	PRINT_INT(this->totalScore, 20 + 7, 0);
	PRINT_TEXT("(X):	   ", 22, 1);
	PRINT_INT(this->scoreMultiplier, 20 + 7, 1);

	if(kPlayerOne == this->playerNumber)
	{
		PRINT_TEXT("Player ", 3, 26);
		PRINT_INT(this->playerNumber, 3 + 7, 26);
	}
	else if(kPlayerTwo == this->playerNumber)
	{
		PRINT_TEXT("Player ", 38, 26);
		PRINT_INT(this->playerNumber, 38 + 7, 26);
	}
}
