/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef TITLE_SCREEN_STATE_H_
#define TITLE_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <GameScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

enum TitleScreenModes
{
	kTitleScreenModeShowPressStart,
	kTitleScreenModeShowOptions,
};

enum TitleScreenOptions
{
	kTitleScreenOptionMarathonMode,
	kTitleScreenOptionChallengeMode,
	kTitleScreenOptionVersusMode,
	kTitleScreenOptionHighscores,
	kTitleScreenOptionOptions,
	kTitleScreenOptionCredits,
};


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class TitleScreenState : GameScreenState
{
	// entity references
	Entity entityMainMenu;
	Entity entityMainMenuVersus;
	Entity entityCursor;
	Entity entityPressStart;
	Entity entityWaitingForOtherPlayer;
	// current state of the main menu
	uint8 mode;
	// currently selected option of the main menu
	uint8 option;

	static TitleScreenState getInstance();

	override void enter(void* owner);
	override void exit(void* owner);
	override void processUserInput(UserInput userInput);
	override void switchState();
}


#endif
