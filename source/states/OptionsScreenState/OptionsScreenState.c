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
#include <Languages.h>
#include <PhysicalWorld.h>
#include <OptionsScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeyPadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <TitleScreenState.h>
#include <AutoPauseManager.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef OPTIONS_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void OptionsScreenState::constructor()
{
	Base::constructor();

	// init members
	this->entityCursor = NULL;
	this->mode = kOptionsScreenModeShowOptions;
	this->option = 0;

	// add event listeners
	Object::addEventListener(this, Object::safeCast(this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void OptionsScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	Base::destructor();
}

// state's enter
void OptionsScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// reset mode
	this->mode = kOptionsScreenModeShowOptions;

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageDefinition*)&OPTIONS_SCREEN_STAGE_ST, NULL, true);

	// get entity references
	this->entityCursor = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "Cursor", true));

	// initial entity states
	OptionsScreenState::updateCursorPosition(this);
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
	OptionsScreenState::updateBrightnessMeter(this);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

// state's exit
void OptionsScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

// state's resume
void OptionsScreenState::resume(void* owner)
{
	Base::resume(this, owner);

	Camera::startEffect(Camera::getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
void OptionsScreenState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

void OptionsScreenState::updateCursorPosition()
{
	Vector3D position =
	{
		__PIXELS_TO_METERS(234),
		__PIXELS_TO_METERS(100 + (this->option * 12)),
		0,
	};
	Entity::setLocalPosition(this->entityCursor, &position);
}

void OptionsScreenState::switchLanguage(bool forward)
{
	// update language
	int numLangs = sizeof(I18n::getLanguages(I18n::getInstance()));
	int language = I18n::getActiveLanguage(I18n::getInstance());
	language = forward
		? (language < (numLangs - 1)) ? language + 1 : 0
		: (language > 0) ? language - 1 : numLangs - 1;
	I18n::setActiveLanguage(I18n::getInstance(), language);
	SaveDataManager::setLanguage(ProgressManager::getInstance(), language);

	// fire event to re-translate all entities
	Object::fireEvent(this, kEventLanguageChanged);
}

void OptionsScreenState::updateAutomaticPauseCheckBox()
{
	bool autoPauseEnabled = AutoPauseManager::isActive(AutoPauseManager::getInstance());
	AnimatedEntity autoPauseCheckBoxEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "APChckBx", true));
	if(autoPauseCheckBoxEntity)
	{
		AnimatedEntity::playAnimation(autoPauseCheckBoxEntity, Utilities::itoa(autoPauseEnabled, 10, 1));
	}
}

void OptionsScreenState::updateBrightnessMeter()
{
	s8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	AnimatedEntity brightnessMeterEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "BrghtnMt", true));
	if(brightnessMeterEntity)
	{
		char* charBrightness = Utilities::itoa(brightnessFactor + 2, 10, 1);
		AnimatedEntity::playAnimation(brightnessMeterEntity, charBrightness);
	}
}

void OptionsScreenState::switchBrightness(bool forward)
{
	// change brightness factor
	s8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	brightnessFactor = forward
    		? (brightnessFactor < 2) ? brightnessFactor + 1 : 2
    		: (brightnessFactor > -2) ? brightnessFactor - 1 : -2;
	BrightnessManager::setBrightnessFactor(BrightnessManager::getInstance(), brightnessFactor);
	ProgressManager::setBrightnessFactor(ProgressManager::getInstance(), brightnessFactor);

	// update visual representation
	OptionsScreenState::updateBrightnessMeter(this);
}

void OptionsScreenState::toggleAutomaticPause()
{
	// (un)set auto pause state
	bool autoPauseEnabled = AutoPauseManager::isActive(AutoPauseManager::getInstance());
	autoPauseEnabled = !autoPauseEnabled;

	AutoPauseManager::setActive(AutoPauseManager::getInstance(), autoPauseEnabled);

	// write state to sram
	SaveDataManager::setAutomaticPauseStatus(ProgressManager::getInstance(), autoPauseEnabled);

	// update visual representation
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
}

void OptionsScreenState::processUserInputModeShowOptions(UserInput userInput)
{
	if((K_A | K_B | K_STA | K_SEL) & userInput.pressedKey)
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
	else if((K_LU | K_RU) & userInput.pressedKey)
	{
		this->option = (this->option > 0) ? this->option - 1 : kOptionScreenOptionBrightness;
		OptionsScreenState::updateCursorPosition(this);
	}
	else if((K_LD | K_RD) & userInput.pressedKey)
	{
		this->option = (this->option < kOptionScreenOptionBrightness) ? this->option + 1 : 0;
		OptionsScreenState::updateCursorPosition(this);
	}
	else if((K_LR | K_RR | K_LL | K_RL) & userInput.pressedKey)
	{
		switch(this->option)
		{
			case kOptionScreenOptionLanguage:
				OptionsScreenState::switchLanguage(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;

			case kOptionScreenOptionAutomaticPause:
				OptionsScreenState::toggleAutomaticPause(this);
				break;

			case kOptionScreenOptionBrightness:
				OptionsScreenState_switchBrightness(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;
		}
	}
}

void OptionsScreenState::processUserInput(UserInput userInput)
{
	switch(this->mode)
	{
		case kOptionsScreenModeShowOptions:
		{
			OptionsScreenState::processUserInputModeShowOptions(this, userInput);
			break;
		}
	}
}

// handle event
void OptionsScreenState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
