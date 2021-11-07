/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef AUTOMATIC_PAUSE_SCREEN_STATE_H_
#define AUTOMATIC_PAUSE_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameScreenState.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class AutomaticPauseScreenState : GameScreenState
{
	static AutomaticPauseScreenState getInstance();
	override void processUserInput(UserInput userInput);
	override void exit(void* owner);
	override void enter(void* owner);
	override void switchState();
}


#endif
