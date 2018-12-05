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

#ifndef PLAYER_H_
#define PLAYER_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Object.h>
#include <GameState.h>
#include <PongBall.h>
#include <PongState.h>
#include <Paddle.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class Player : Object
{
	/* spec pointer */
	PongBall pongBall;
	VirtualList playerPaddles;
	VirtualList opponentPaddles;
	u32 leftScore;
	u32 rightScore;
	u32 totalLeftScore;
	u32 totalRightScore;
	u32 totalScore;
	int scoreMultiplierThreshold;
	u32 scoreMultiplier;
	bool ballIsRolling;
	int playerNumber;

	static Player getInstance();
	void getReady(GameState gameState);
	void gameIsOver(GameState gameState);
	void printScore();
	int getPlayerNumber();
	PongBall getPongBall();
	override bool handleMessage(Telegram telegram);
}

enum PlayerNumbers
{
	kPlayerAlone = 0,
	kPlayerOne,
	kPlayerTwo
};


#endif
