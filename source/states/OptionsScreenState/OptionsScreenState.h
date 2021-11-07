/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef OPTIONS_SCREEN_STATE_H_
#define OPTIONS_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

enum OptionsScreenModes
{
	kOptionsScreenModeShowOptions,
};

enum OptionsScreenOptions
{
	kOptionScreenOptionLanguage,
	kOptionScreenOptionAutomaticPause,
	kOptionScreenOptionBrightness,
};


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class OptionsScreenState : GameScreenState
{
	// entity references
	Entity entityCursor;
	// current state of the menu
	uint8 mode;
	// currently selected option of the menu
	uint8 option;

	static OptionsScreenState getInstance();

	override void enter(void* owner);
	override void exit(void* owner);
	override void processUserInput(UserInput userInput);
	override void switchState();
}


#endif
