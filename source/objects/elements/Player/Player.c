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
#include <GameEvents.h>
#include <debugUtilities.h>
#include "Player.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define SCORE_MULTIPLIER_THRESHOLD		5


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(Player, Object);


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void Player_constructor(Player this);
static void Player_onUserInput(Player this, Object eventFirer __attribute__ ((unused)));
static void Player_onKeyPressed(Player this, const UserInput* userInput);
static void Player_onKeyReleased(Player this, const UserInput* userInput);
static void Player_onKeyHold(Player this, const UserInput* userInput);
static void Player_movePaddles(Player this, s8 horizontalInput, s8 verticalInput);
static void Player_stopPaddles(Player this, s8 horizontalInput, s8 verticalInput);
static void Player_retractPaddles(Player this);
static void Player_ejectPaddles(Player this);
static void Player_onPongBallHitFloor(Player this, Object eventFirer __attribute__ ((unused)));
static void Player_onPongBallHitCeiling(Player this, Object eventFirer __attribute__ ((unused)));
static void Player_onPongBallHitPaddle(Player this, Object eventFirer __attribute__ ((unused)));


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

__SINGLETON(Player);

static void __attribute__ ((noinline)) Player_constructor(Player this)
{
	ASSERT(this, "Player::constructor: null this");

	__CONSTRUCT_BASE(Object);

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
}

void Player_destructor(Player this)
{
	ASSERT(this, "Player::destructor: null this");

	this->pongBall = NULL;
	this->paddles[kLeftPaddle] = NULL;
	this->paddles[kRightPaddle] = NULL;

	// allow a new construct
	__SINGLETON_DESTROY;
}

bool Player_handleMessage(Player this __attribute__ ((unused)), Telegram telegram __attribute__ ((unused)))
{
	ASSERT(this, "Player::handleMessage: null this");

	return false;
}


void Player_getReady(Player this, GameState gameState)
{
	ASSERT(this, "Player::getReady: null this");

	this->pongBall = __SAFE_CAST(PongBall, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), (char*)PONG_BALL_NAME, true));
	this->paddles[kLeftPaddle] = __SAFE_CAST(Paddle, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), (char*)PADDLE_LEFT_NAME, true));
	this->paddles[kRightPaddle] = __SAFE_CAST(Paddle, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), (char*)PADDLE_RIGHT_NAME, true));

	ASSERT(this->paddles[kLeftPaddle], "Player::getReady: left paddle not found");
	ASSERT(this->paddles[kRightPaddle], "Player::getReady: right paddle not found");

	Object_addEventListener(__SAFE_CAST(Object, gameState), __SAFE_CAST(Object, this), (EventListener)Player_onUserInput, kEventUserInput);
	Object_addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object_addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object_addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);

	Player_printScore(this);
	KeypadManager_registerInput(KeypadManager_getInstance(), __KEY_PRESSED | __KEY_RELEASED | __KEY_HOLD);
}

void Player_gameIsOver(Player this, GameState gameState)
{
	ASSERT(this, "Player::gameIsOver: null this");

	this->paddles[kLeftPaddle] = NULL;
	this->paddles[kRightPaddle] = NULL;

	Object_removeEventListener(__SAFE_CAST(Object, gameState), __SAFE_CAST(Object, this), (EventListener)Player_onUserInput, kEventUserInput);
	Object_removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitFloor, kEventPongBallHitFloor);
	Object_removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitCeiling, kEventPongBallHitCeiling);
	Object_removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)Player_onPongBallHitPaddle, kEventPongBallHitPaddle);
}

// process user input
static void Player_onUserInput(Player this, Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "Player::onUserInput: null this");
	ASSERT(__SAFE_CAST(PongState, eventFirer), "Player::onUserInput: wrong event firer");

	UserInput userInput = KeypadManager_getUserInput(KeypadManager_getInstance());

	if(userInput.pressedKey)
	{
		Player_onKeyPressed(this, &userInput);
	}

	if(userInput.releasedKey)
	{
		Player_onKeyReleased(this, &userInput);
	}

	if(userInput.holdKey)
	{
		Player_onKeyHold(this, &userInput);
	}
}

static void Player_onKeyPressed(Player this __attribute__ ((unused)), const UserInput* userInput)
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

static void Player_onKeyReleased(Player this __attribute__ ((unused)), const UserInput* userInput)
{
	if(K_B & userInput->releasedKey)
	{
	}

	if(K_A & userInput->releasedKey)
	{
		Player_ejectPaddles(this);
	}

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
		Player_stopPaddles(this, horizontalInput, verticalInput);
	}
}

static void Player_onKeyHold(Player this __attribute__ ((unused)), const UserInput* userInput)
{
	if(K_B & userInput->holdKey)
	{
	}

	if(K_A & userInput->holdKey)
	{
		Player_retractPaddles(this);
	}

	s8 horizontalInput = 0;
	s8 verticalInput = 0;

	// check direction
	if(K_LL & userInput->holdKey)
	{
		horizontalInput = -1;
	}
	else if(K_LR & userInput->holdKey)
	{
		horizontalInput = 1;
	}

	if(K_LU & userInput->holdKey)
	{
		verticalInput = -1;
	}
	else if(K_LD & userInput->holdKey)
	{
		verticalInput = 1;
	}

	if(horizontalInput || verticalInput)
	{
		Player_movePaddles(this, horizontalInput, verticalInput);
	}
}

static void Player_movePaddles(Player this, s8 horizontalInput, s8 verticalInput)
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
			Paddle_moveTowards(this->paddles[i], direction);
		}
	}
}

static void Player_stopPaddles(Player this, s8 horizontalInput, s8 verticalInput)
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
			//Paddle_stopTowards(this->paddles[i], direction);
		}
	}
}

static void Player_retractPaddles(Player this)
{
	int i = 0;

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			Paddle_retract(this->paddles[i]);
		}
	}
}

static void Player_ejectPaddles(Player this)
{
	int i = 0;

	for(; i < 2; i++)
	{
		if(this->paddles[i])
		{
			Paddle_eject(this->paddles[i]);
		}
	}
}

static void Player_onPongBallHitFloor(Player this, Object eventFirer __attribute__ ((unused)))
{
	switch(PongBall_getPaddleEnum(this->pongBall))
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

	this->totalLeftScore += this->scoreMultiplier * this->leftScore;
	this->totalRightScore += this->scoreMultiplier * this->rightScore;
	this->scoreMultiplier = 1;
	this->leftScore = 0;
	this->rightScore = 0;
	this->totalScore = this->totalLeftScore + this->totalRightScore;

	Player_printScore(this);
}

static void Player_onPongBallHitCeiling(Player this, Object eventFirer __attribute__ ((unused)))
{
	switch(PongBall_getPaddleEnum(this->pongBall))
	{
		case kLeftPaddle:

			this->leftScore += 5;
			break;

		case kRightPaddle:

			this->rightScore += 5;
			break;
	}

	if(0 >= --this->scoreMultiplierThreshold)
	{
		this->scoreMultiplierThreshold = SCORE_MULTIPLIER_THRESHOLD;
		this->scoreMultiplier += 2;
	}

	Player_printScore(this);
}

static void Player_onPongBallHitPaddle(Player this, Object eventFirer __attribute__ ((unused)))
{
	switch(PongBall_getPaddleEnum(this->pongBall))
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

	Player_printScore(this);
}

void Player_printScore(Player this)
{
	PRINT_TEXT("Total:      ", 1, 0);
	PRINT_INT(this->totalLeftScore, 10, 0);
	PRINT_TEXT("Current:    ", 1, 1);
	PRINT_INT(this->leftScore, 10, 1);

	PRINT_TEXT("Total:      ", 35, 0);
	PRINT_INT(this->totalRightScore, 35 + 9, 0);
	PRINT_TEXT("Current: ", 35, 1);
	PRINT_INT(this->rightScore, 35 + 9, 1);

	PRINT_TEXT("TOTAL:     ", 20, 0);
	PRINT_INT(this->totalScore, 20 + 7, 0);
	PRINT_TEXT("(X):       ", 22, 1);
	PRINT_INT(this->scoreMultiplier, 20 + 7, 1);

}
