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
#include "Player.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define SCORE_MULTIPLIER_THRESHOLD			5
#define BONUS_INCREMENT_DELAY				100
#define SCORE_MULTIPLIER_TO_ENABLE_BONUS	5


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

void Player::constructor(Player this);
static void Player::onUserInput(Player this, Object eventFirer __attribute__ ((unused)));
static void Player::onKeyPressed(Player this, const UserInput* userInput);
static void Player::onKeyReleased(Player this, const UserInput* userInput);
static void Player::onKeyHold(Player this, const UserInput* userInput);
static void Player::movePaddles(Player this, s8 horizontalInput, s8 verticalInput);
static void Player::stopPaddles(Player this, s8 horizontalInput, s8 verticalInput);
static void Player::retractPaddles(Player this);
static void Player::ejectPaddles(Player this);
static void Player::onPongBallHitFloor(Player this, Object eventFirer __attribute__ ((unused)));
static void Player::onPongBallHitCeiling(Player this, Object eventFirer __attribute__ ((unused)));
static void Player::onPongBallHitPaddle(Player this, Object eventFirer __attribute__ ((unused)));
static void Player::totalizeScore(Player this);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------



void __attribute__ ((noinline)) Player::constructor(Player this)
{
	ASSERT(this, "Player::constructor: null this");

	Base::constructor();

	this->pongBall = NULL;
	this->paddles[kLeftPaddle] = NULL;
	this->paddles[kRightPaddle] = NULL;

	this->leftScore = 0;
	this->rightScore = 0;
	this->totalLeftScore = 0;
	this->totalRightScore = 0;
	this->totalScore = 0;
	this->scoreMultiplier = 1;
	this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
	this->ballIsRolling = false;
}

void Player::destructor(Player this)
{
	ASSERT(this, "Player::destructor: null this");

	this->pongBall = NULL;
	this->paddles[kLeftPaddle] = NULL;
	this->paddles[kRightPaddle] = NULL;

	// allow a new construct
	__SINGLETON_DESTROY;
}

bool Player::handleMessage(Player this __attribute__ ((unused)), Telegram telegram __attribute__ ((unused)))
{
	ASSERT(this, "Player::handleMessage: null this");

	switch(Telegram::getMessage(telegram))
	{
		case kAddBonusScore:

			this->leftScore += 1;
			this->rightScore += 1;
			Player::printScore(this);

			MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kAddBonusScore, NULL);
			break;
	}

	return false;
}


void Player::getReady(Player this, GameState gameState)
{
	ASSERT(this, "Player::getReady: null this");

	this->pongBall = __SAFE_CAST(PongBall, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), (char*)PONG_BALL_NAME, true));
	this->paddles[kLeftPaddle] = __SAFE_CAST(Paddle, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), (char*)PADDLE_LEFT_NAME, true));
	this->paddles[kRightPaddle] = __SAFE_CAST(Paddle, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), (char*)PADDLE_RIGHT_NAME, true));

	ASSERT(this->paddles[kLeftPaddle], "Player::getReady: left paddle not found");
	ASSERT(this->paddles[kRightPaddle], "Player::getReady: right paddle not found");

	Object::addEventListener(__SAFE_CAST(Object, gameState), __SAFE_CAST(Object, this), (EventListener)Player_onUserInput, kEventUserInput);
	Object::addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object::addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);

	Player::printScore(this);
	KeypadManager::registerInput(KeypadManager::getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);
}

void Player::gameIsOver(Player this, GameState gameState)
{
	ASSERT(this, "Player::gameIsOver: null this");

	this->paddles[kLeftPaddle] = NULL;
	this->paddles[kRightPaddle] = NULL;

	Object::removeEventListener(__SAFE_CAST(Object, gameState), __SAFE_CAST(Object, this), (EventListener)Player_onUserInput, kEventUserInput);
	Object::removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object::removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);
}

// process user input
static void Player::onUserInput(Player this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "Player::onUserInput: null this");
	ASSERT(__SAFE_CAST(PongState, eventFirer), "Player::onUserInput: wrong event firer");

	UserInput userInput = KeypadManager::getUserInput(KeypadManager::getInstance());

	if(userInput.pressedKey)
	{
		Player::onKeyPressed(this, &userInput);
	}

	if(userInput.releasedKey)
	{
		Player::onKeyReleased(this, &userInput);
	}

	if(userInput.holdKey)
	{
		Player::onKeyHold(this, &userInput);
	}
}

static void Player::onKeyPressed(Player this __attribute__ ((unused)), const UserInput* userInput)
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

static void Player::onKeyReleased(Player this __attribute__ ((unused)), const UserInput* userInput)
{
	if(K_B & userInput->releasedKey)
	{
	}

	if(K_A & userInput->releasedKey)
	{
		Player::ejectPaddles(this);
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

static void Player::onKeyHold(Player this __attribute__ ((unused)), const UserInput* userInput)
{
	if(K_B & userInput->holdKey)
	{
	}

	if(K_A & userInput->holdKey)
	{
		Player::retractPaddles(this);
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

	if((K_RU) & userInput->holdKey)
	{
		verticalInput = -1;
	}
	else if((K_RD) & userInput->holdKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player::movePaddles(this, horizontalInput, verticalInput);
	}
}

static void Player::movePaddles(Player this, s8 horizontalInput, s8 verticalInput)
{
	int i = 0;

	Direction direction =
	{
		horizontalInput,
		verticalInput,
		0
	};

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			Paddle::moveTowards(this->paddles[i], direction);
		}
	}
}

static void Player::stopPaddles(Player this, s8 horizontalInput, s8 verticalInput)
{
	int i = 0;

	Direction direction =
	{
		horizontalInput,
		verticalInput,
		0
	};

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			//Paddle::stopTowards(this->paddles[i], direction);
		}
	}
}

static void Player::retractPaddles(Player this)
{
	int i = 0;

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			Paddle::retract(this->paddles[i]);
		}
	}
}

static void Player::ejectPaddles(Player this)
{
	int i = 0;

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			Paddle::eject(this->paddles[i]);
		}
	}
}

static void Player::onPongBallHitFloor(Player this, Object eventFirer __attribute__ ((unused)))
{
	if(this->scoreMultiplier >= SCORE_MULTIPLIER_TO_ENABLE_BONUS)
	{
		PongBall::startRolling(this->pongBall);
		this->ballIsRolling = true;
		MessageDispatcher::dispatchMessage(BONUS_INCREMENT_DELAY, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kAddBonusScore, NULL);
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

static void Player::totalizeScore(Player this)
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

static void Player::onPongBallHitCeiling(Player this, Object eventFirer __attribute__ ((unused)))
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

static void Player::onPongBallHitPaddle(Player this, Object eventFirer __attribute__ ((unused)))
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

		MessageDispatcher::discardDelayedMessagesFromSender(MessageDispatcher::getInstance(), __SAFE_CAST(Object, this), kAddBonusScore);
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

void Player::printScore(Player this)
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

}
