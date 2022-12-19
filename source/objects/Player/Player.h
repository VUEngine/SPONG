/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
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

singleton class Player : ListenerObject
{
	/* spec pointer */
	PongBall pongBall;
	VirtualList playerPaddles;
	VirtualList opponentPaddles;
	uint32 leftScore;
	uint32 rightScore;
	uint32 totalLeftScore;
	uint32 totalRightScore;
	uint32 totalScore;
	int scoreMultiplierThreshold;
	uint32 scoreMultiplier;
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
