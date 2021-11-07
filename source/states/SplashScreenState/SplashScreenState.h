/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef SPLASH_SCREEN_STATE_H_
#define SPLASH_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameScreenState.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S ENUMS
//---------------------------------------------------------------------------------------------------------

enum SplashScreensMessageTypes
{
	kScreenStarted = kMessageLastEngine + 1,
	kScreenAllowUserInput,
};


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

abstract class SplashScreenState : GameScreenState
{
	// state to enter after this one
	GameState nextState;
	// spec of screen's stage
	StageSpec* stageSpec;

	void constructor();
	void setNextState(GameState nextState);
	void loadNextState();
	virtual void print();
	virtual void processInput(uint32 releasedKey);
	override void enter(void* owner);
	override void exit(void* owner);
	override bool processMessage(void* owner, Telegram telegram);
	override void processUserInput(UserInput userInput);
	override void switchState();
}


#endif
