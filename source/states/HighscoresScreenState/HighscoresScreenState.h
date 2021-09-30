/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef HIGHSCORES_SCREEN_STATE_H_
#define HIGHSCORES_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <SpongScreenState.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class HighscoresScreenState : SpongScreenState
{
	static HighscoresScreenState getInstance();
	override void enter(void* owner);
	override void exit(void* owner);
	override void processUserInput(UserInput userInput);
	override void switchState();
}


#endif
