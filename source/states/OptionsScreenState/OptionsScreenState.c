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
#include <AutoPauseScreenState.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef OPTIONS_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------


void OptionsScreenState::constructor(OptionsScreenState this);
void OptionsScreenState::toggleAutomaticPause(OptionsScreenState this);
void OptionsScreenState::switchBrightness(OptionsScreenState this, bool forward);
void OptionsScreenState::updateAutomaticPauseCheckBox(OptionsScreenState this);
void OptionsScreenState::updateBrightnessMeter(OptionsScreenState this);
void OptionsScreenState::updateCursorPosition(OptionsScreenState this);
static void OptionsScreenState::onTransitionOutComplete(OptionsScreenState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------





//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void __attribute__ ((noinline)) OptionsScreenState::constructor(OptionsScreenState this)
{
	Base::constructor();

	// init members
	this->entityCursor = NULL;
	this->mode = kOptionsScreenModeShowOptions;
	this->option = 0;

	// add event listeners
	Object::addEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void OptionsScreenState::destructor(OptionsScreenState this)
{
	// remove event listeners
	Object::removeEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void OptionsScreenState::enter(OptionsScreenState this, void* owner)
{
	// reset mode
	this->mode = kOptionsScreenModeShowOptions;

	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&OPTIONS_SCREEN_STAGE_ST, NULL, true);

	// get entity references
	this->entityCursor = __SAFE_CAST(Entity, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), "Cursor", true));

	// initial entity states
	OptionsScreenState::updateCursorPosition(this);
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
	OptionsScreenState::updateBrightnessMeter(this);

	// start clocks to start animations
	GameState::startClocks(__SAFE_CAST(GameState, this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::showScreen(BrightnessManager::getInstance());
}

// state's exit
void OptionsScreenState::exit(OptionsScreenState this, void* owner)
{
	// call base
	Base::exit(this, owner);
}

// state's resume
void OptionsScreenState::resume(OptionsScreenState this, void* owner)
{
	Base::resume(this, owner);

	Camera::startEffect(Camera::getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
void OptionsScreenState::suspend(OptionsScreenState this, void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

void OptionsScreenState::updateCursorPosition(OptionsScreenState this)
{
	Vector3D position =
	{
		__PIXELS_TO_METERS(234),
		__PIXELS_TO_METERS(100 + (this->option * 12)),
		0,
	};
	Entity::setLocalPosition(this->entityCursor, &position);
}

void OptionsScreenState::switchLanguage(OptionsScreenState this __attribute__ ((unused)), bool forward)
{
	// update language
	int numLangs = sizeof(I18n::getLanguages(I18n::getInstance()));
	int language = I18n::getActiveLanguage(I18n::getInstance());
	language = forward
		? (language < (numLangs - 1)) ? language + 1 : 0
		: (language > 0) ? language - 1 : numLangs - 1;
	I18n::setActiveLanguage(I18n::getInstance(), language);
	ProgressManager::setLanguage(ProgressManager::getInstance(), language);

	// fire event to re-translate all entities
	Object::fireEvent(__SAFE_CAST(Object, this), kEventLanguageChanged);
}

void OptionsScreenState::updateAutomaticPauseCheckBox(OptionsScreenState this __attribute__ ((unused)))
{
	bool autoPauseEnabled = (Game::getAutomaticPauseState(Game::getInstance()) != NULL);
	AnimatedEntity autoPauseCheckBoxEntity = __SAFE_CAST(AnimatedEntity, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), "APChckBx", true));
	if(autoPauseCheckBoxEntity)
	{
		AnimatedEntity::playAnimation(autoPauseCheckBoxEntity, Utilities::itoa(autoPauseEnabled, 10, 1));
	}
}

void OptionsScreenState::updateBrightnessMeter(OptionsScreenState this __attribute__ ((unused)))
{
	u8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	AnimatedEntity brightnessMeterEntity = __SAFE_CAST(AnimatedEntity, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), "BrghtnMt", true));
	if(brightnessMeterEntity)
	{
		char* charBrightness = Utilities::itoa(brightnessFactor, 10, 1);
		AnimatedEntity::playAnimation(brightnessMeterEntity, charBrightness);
	}
}

void OptionsScreenState::switchBrightness(OptionsScreenState this __attribute__ ((unused)), bool forward)
{
	// change brightness factor
	u8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	brightnessFactor = forward
    		? (brightnessFactor < 4) ? brightnessFactor + 1 : 4
    		: (brightnessFactor > 0) ? brightnessFactor - 1 : 0;
	BrightnessManager::setBrightnessFactor(BrightnessManager::getInstance(), brightnessFactor);
	ProgressManager::setBrightnessFactor(ProgressManager::getInstance(), brightnessFactor);

	// update visual representation
	OptionsScreenState::updateBrightnessMeter(this);
}

void OptionsScreenState::toggleAutomaticPause(OptionsScreenState this)
{
	// (un)set auto pause state
	bool autoPauseEnabled = (Game::getAutomaticPauseState(Game::getInstance()) != NULL);
	autoPauseEnabled = !autoPauseEnabled;
	Game::setAutomaticPauseState(Game::getInstance(), autoPauseEnabled
		? __SAFE_CAST(GameState, AutoPauseScreenState::getInstance())
		: NULL
	);

	// write state to sram
	ProgressManager::setAutomaticPauseStatus(ProgressManager::getInstance(), autoPauseEnabled);

	// update visual representation
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
}

void OptionsScreenState::processUserInputModeShowOptions(OptionsScreenState this, UserInput userInput)
{
	if((K_A | K_B | K_STA | K_SEL) & userInput.pressedKey)
	{
		// disable user input
		Game::disableKeypad(Game::getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = __SAFE_CAST(AnimatedEntity, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), "TRNSLYR", true));
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
				OptionsScreenState::switchLanguage(this, (K_LR | K_RR) & userInput.pressedKey);
				break;

			case kOptionScreenOptionAutomaticPause:
				OptionsScreenState::toggleAutomaticPause(this);
				break;

			case kOptionScreenOptionBrightness:
				OptionsScreenState::switchBrightness(this, (K_LR | K_RR) & userInput.pressedKey);
				break;
		}
	}
}

void OptionsScreenState::processUserInput(OptionsScreenState this, UserInput userInput)
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
static void OptionsScreenState::onTransitionOutComplete(OptionsScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "OptionsScreenState::onTransitionOutComplete: null this");

	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	Game::changeState(Game::getInstance(), __SAFE_CAST(GameState, TitleScreenState::getInstance()));
}
