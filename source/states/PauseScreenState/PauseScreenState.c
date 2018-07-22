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
#include <I18n.h>
#include <PhysicalWorld.h>
#include <PauseScreenState.h>
#include <Languages.h>
#include <KeyPadManager.h>
#include <Utilities.h>
#include <AnimatedEntity.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef PAUSE_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void PauseScreenState::constructor()
{
	Base::constructor();

	// init members
	this->mode = kPauseScreenModeShowOptions;
	this->optionsSelector = new OptionsSelector(1, 3, NULL);

	// add event listeners
	Object::addEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)PauseScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void PauseScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)PauseScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

	delete this->optionsSelector;

	// destroy base
	Base::destructor();
}

// state's enter
void PauseScreenState::enter(void* owner __attribute__ ((unused)))
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageDefinition*)&PAUSE_SCREEN_STAGE_ST, NULL, true);

	// print pause text
/*
	const char* strPause = I18n::getText(I18n::getInstance(), STR_PAUSE);
	FontSize strPauseSize = Printing::getTextSize(Printing::getInstance(), strPause, NULL);
	Printing::text(
		Printing::getInstance(),
		Utilities::toUppercase(strPause),
		(((__SCREEN_WIDTH_IN_CHARS) - strPauseSize.x) >> 1),
		14,
		NULL
	);
*/

	// show menu
	/*
	VirtualList options = new VirtualList();
	Option* option = NULL;

	option = new Option;
	option->value = "...";
	option->type = kString;
	option->callback = NULL;
	option->callbackScope = NULL;
	VirtualList::pushBack(options, option);

	option = new Option;
	option->value = (char*)I18n::getText(I18n::getInstance(), STR_OPTIONS);
	option->type = kString;
	option->callback = NULL;
	option->callbackScope = NULL;
	VirtualList::pushBack(options, option);

	option = new Option;
	option->value = "...";
	option->type = kString;
	option->callback = NULL;
	option->callbackScope = NULL;
	VirtualList::pushBack(options, option);

	OptionsSelector::setOptions(this->optionsSelector, options);
	delete options;

	OptionsSelector::printOptions(
		this->optionsSelector,
		(((__SCREEN_WIDTH_IN_CHARS) - strPauseSize.x) >> 1) - 1,
		17
	);*/

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::showScreen(BrightnessManager::getInstance());

	this->mode = kPauseScreenModeShowOptions;
}

// state's exit
void PauseScreenState::exit(void* owner __attribute__ ((unused)))
{
	// call base
	Base::exit(this, owner);
}

void PauseScreenState::processUserInput(UserInput userInput)
{
	if((K_STA | K_A) & userInput.pressedKey)
	{
		if(this->mode == kPauseScreenModeShowOptions)
		{
			int selectedOption = OptionsSelector::getSelectedOption(this->optionsSelector);

			switch(selectedOption)
			{
				case kPauseScreenOptionContinue:
				case kPauseScreenOptionOptions:

					// disable user input
					Game::disableKeypad(Game::getInstance());

					// transition layer animation
					AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
					if(transitionLayerEntity)
					{
						AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
					}

					break;

				case kPauseScreenOptionQuitLevel:
				{
					// print confirmation message
					/*
					const char* strYes = I18n::getText(I18n::getInstance(), STR_YES);
					FontSize strYesSize = Printing::getTextSize(Printing::getInstance(), strYes, NULL);
					const char* strNo = I18n::getText(I18n::getInstance(), STR_NO);
					const char* strAreYouSure = I18n::getText(I18n::getInstance(), STR_ARE_YOU_SURE);
					const char* strPause = I18n::getText(I18n::getInstance(), STR_PAUSE);
					FontSize strPauseSize = Printing::getTextSize(Printing::getInstance(), strPause, NULL);

					u8 strXPos = ((__SCREEN_WIDTH_IN_CHARS) - strPauseSize.x) >> 1;
					u8 strNoXPos = strXPos + strYesSize.x + 2;

					Printing::text(Printing::getInstance(), strAreYouSure, strXPos, 21, NULL);
					Printing::text(Printing::getInstance(), __CHAR_A_BUTTON, strXPos, 22, NULL);
					Printing::text(Printing::getInstance(), strYes, strXPos + 1, 22, NULL);
					Printing::text(Printing::getInstance(), __CHAR_B_BUTTON, strNoXPos, 22, NULL);
					Printing::text(Printing::getInstance(), strNo, strNoXPos + 1, 22, NULL);
					*/

					// set mode accordingly
					this->mode = kPauseScreenModeShowConfirmQuit;
					break;
				}
			}
		}
		else if(this->mode == kPauseScreenModeShowConfirmQuit)
		{
			// disable user input
			Game::disableKeypad(Game::getInstance());

			// transition layer animation
			AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
			if(transitionLayerEntity)
			{
				AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
			}
		}
	}
	else if((this->mode == kPauseScreenModeShowConfirmQuit) && (userInput.pressedKey & K_B))
	{
		// remove confirmation message
		Printing::text(Printing::getInstance(), "                                                ", 0, 21, NULL);
		Printing::text(Printing::getInstance(), "                                                ", 0, 22, NULL);

		// set mode back to main menu
		this->mode = kPauseScreenModeShowOptions;
	}
	else if((this->mode == kPauseScreenModeShowOptions) && ((userInput.pressedKey & K_LU) || (userInput.pressedKey & K_RU)))
	{
		OptionsSelector::selectPrevious(this->optionsSelector);
	}
	else if((this->mode == kPauseScreenModeShowOptions) && ((userInput.pressedKey & K_LD) || (userInput.pressedKey & K_RD)))
	{
		OptionsSelector::selectNext(this->optionsSelector);
	}
}

// handle event
void PauseScreenState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	// re-enable user input
	Game::enableKeypad(Game::getInstance());

	// switch state according to selection
	int selectedOption = OptionsSelector::getSelectedOption(this->optionsSelector);
	switch(selectedOption)
	{
		case kPauseScreenOptionContinue:

			// resume game
			Game::unpause(Game::getInstance(), GameState::safeCast(this));
			break;
/*
		case kPauseScreenOptionOptions:

			// switch to options state
			OptionsScreenState::setNextState(OptionsScreenState::getInstance(), GameState::safeCast(this));
			Game::changeState(Game::getInstance(), GameState::safeCast(OptionsScreenState::getInstance()));
			break;
*/
		case kPauseScreenOptionQuitLevel:

			// switch to overworld after deleting paused game state
			//Game::cleanAndChangeState(Game::getInstance(), GameState::safeCast(PlatformerLevelState::getInstance()));

			break;
	}
}
