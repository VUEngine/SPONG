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

#include <string.h>
#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <PhysicalWorld.h>
#include <I18n.h>
#include <AutoPauseScreenState.h>
#include <KeypadManager.h>
#include <Languages.h>
#include <Utilities.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef PAUSE_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------


void AutoPauseScreenState::constructor(AutoPauseScreenState this);
static void AutoPauseScreenState::onTransitionOutComplete(AutoPauseScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)));


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------


__SINGLETON_DYNAMIC(AutoPauseScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void __attribute__ ((noinline)) AutoPauseScreenState::constructor(AutoPauseScreenState this)
{
	Base::constructor();

	// add event listeners
	Object::addEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)AutoPauseScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void AutoPauseScreenState::destructor(AutoPauseScreenState this)
{
	// remove event listeners
    Object::removeEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)AutoPauseScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void AutoPauseScreenState::enter(AutoPauseScreenState this, void* owner __attribute__ ((unused)))
{
	// load stage
	GameState::loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&PAUSE_SCREEN_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState::startClocks(__SAFE_CAST(GameState, this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::showScreen(BrightnessManager::getInstance());
}

// state's exit
void AutoPauseScreenState::exit(AutoPauseScreenState this __attribute__ ((unused)), void* owner __attribute__ ((unused)))
{
	// call base
	Base::exit(this, owner);

	// destroy the state
	__DELETE(this);
}

void AutoPauseScreenState::processUserInput(AutoPauseScreenState this __attribute__ ((unused)), UserInput userInput)
{
	if(K_STA & userInput.pressedKey)
	{
		// disable user input
		Game::disableKeypad(Game::getInstance());
	}
}

// handle event
static void AutoPauseScreenState::onTransitionOutComplete(AutoPauseScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "AutoPauseScreenState::onTransitionOutComplete: null this");

	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	// re-enable user input
	Game::enableKeypad(Game::getInstance());

	// resume game
	Game::unpause(Game::getInstance(), __SAFE_CAST(GameState, this));
}
