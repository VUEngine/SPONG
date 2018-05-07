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
#include <EventManager.h>
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

static void OptionsScreenState_destructor(OptionsScreenState this);
static void OptionsScreenState_constructor(OptionsScreenState this);
static void OptionsScreenState_enter(OptionsScreenState this, void* owner);
static void OptionsScreenState_exit(OptionsScreenState this, void* owner);
static void OptionsScreenState_resume(OptionsScreenState this, void* owner);
static void OptionsScreenState_suspend(OptionsScreenState this, void* owner);
void OptionsScreenState_switchLanguage(OptionsScreenState this, bool forward);
void OptionsScreenState_toggleAutomaticPause(OptionsScreenState this);
void OptionsScreenState_switchBrightness(OptionsScreenState this, bool forward);
void OptionsScreenState_processUserInputModeShowOptions(OptionsScreenState this, UserInput userInput);
void OptionsScreenState_updateAutomaticPauseCheckBox(OptionsScreenState this);
void OptionsScreenState_updateCursorPosition(OptionsScreenState this);
static void OptionsScreenState_onTransitionOutComplete(OptionsScreenState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(OptionsScreenState, GameState);
__SINGLETON(OptionsScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) OptionsScreenState_constructor(OptionsScreenState this)
{
	__CONSTRUCT_BASE(GameState);

	// init members
	this->entityCursor = NULL;
	this->mode = kOptionsScreenModeShowOptions;
	this->option = 0;

	// add event listeners
	Object_addEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
static void OptionsScreenState_destructor(OptionsScreenState this)
{
	// remove event listeners
	Object_removeEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)OptionsScreenState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void OptionsScreenState_enter(OptionsScreenState this, void* owner)
{
	// reset mode
	this->mode = kOptionsScreenModeShowOptions;

	// call base
	__CALL_BASE_METHOD(GameState, enter, this, owner);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&OPTIONS_SCREEN_STAGE_ST, NULL, true);

	// get entity references
	this->entityCursor = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "Cursor", true));

	// initial entity states
	OptionsScreenState_updateCursorPosition(this);
	OptionsScreenState_updateAutomaticPauseCheckBox(this);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// enable user input
	Game_enableKeypad(Game_getInstance());

	// show screen
	BrightnessManager_showScreen(BrightnessManager_getInstance());
}

// state's exit
static void OptionsScreenState_exit(OptionsScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, exit, this, owner);
}

// state's resume
static void OptionsScreenState_resume(OptionsScreenState this, void* owner)
{
	__CALL_BASE_METHOD(GameState, resume, this, owner);

	Camera_startEffect(Camera_getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
static void OptionsScreenState_suspend(OptionsScreenState this, void* owner)
{
	Camera_startEffect(Camera_getInstance(), kFadeOut, __FADE_DELAY);

	__CALL_BASE_METHOD(GameState, suspend, this, owner);
}

void OptionsScreenState_updateCursorPosition(OptionsScreenState this)
{
	Vector3D position =
	{
		__PIXELS_TO_METERS(234),
		__PIXELS_TO_METERS(100 + (this->option * 12)),
		0,
	};
	Entity_setLocalPosition(this->entityCursor, &position);
}

void OptionsScreenState_switchLanguage(OptionsScreenState this __attribute__ ((unused)), bool forward)
{
	int numLangs = sizeof(I18n_getLanguages(I18n_getInstance()));
	int language = I18n_getActiveLanguage(I18n_getInstance());
	language = forward
		? (language < (numLangs - 1)) ? language + 1 : 0
		: (language > 0) ? language - 1 : numLangs - 1;
	I18n_setActiveLanguage(I18n_getInstance(), language);
	ProgressManager_setLanguage(ProgressManager_getInstance(), language);

	// fire event to re-translate all entities
	Object_fireEvent(__SAFE_CAST(Object, EventManager_getInstance()), kEventLanguageChanged);
}

void OptionsScreenState_updateAutomaticPauseCheckBox(OptionsScreenState this __attribute__ ((unused)))
{
	bool autoPauseEnabled = (Game_getAutomaticPauseState(Game_getInstance()) != NULL);
	AnimatedEntity autoPauseCheckBoxEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "APChckBx", true));
	if(autoPauseCheckBoxEntity)
	{
		AnimatedEntity_playAnimation(autoPauseCheckBoxEntity, Utilities_itoa(autoPauseEnabled, 10, 1));
	}
}

void OptionsScreenState_switchBrightness(OptionsScreenState this __attribute__ ((unused)), bool forward)
{
	u8 brightnessFactor = BrightnessManager_getBrightnessFactor(BrightnessManager_getInstance());
	brightnessFactor = forward
    		? (brightnessFactor < 4) ? brightnessFactor + 1 : 4
    		: (brightnessFactor > 0) ? brightnessFactor - 1 : 0;
	BrightnessManager_setBrightnessFactor(BrightnessManager_getInstance(), brightnessFactor);
	//ProgressManager_setBrightnessFactor(ProgressManager_getInstance(), brightnessFactor);

	AnimatedEntity brightnessMeterEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "BrgthnMt", true));
	if(brightnessMeterEntity)
	{
		char* charBrightness = Utilities_itoa(brightnessFactor, 10, 1);
		AnimatedEntity_playAnimation(brightnessMeterEntity, charBrightness);
	}
}

void OptionsScreenState_toggleAutomaticPause(OptionsScreenState this)
{
	// (un)set auto pause state
	bool autoPauseEnabled = (Game_getAutomaticPauseState(Game_getInstance()) != NULL);
	autoPauseEnabled = !autoPauseEnabled;
	Game_setAutomaticPauseState(Game_getInstance(), autoPauseEnabled
		? __SAFE_CAST(GameState, AutoPauseScreenState_getInstance())
		: NULL
	);

	// write state to sram
	ProgressManager_setAutomaticPauseStatus(ProgressManager_getInstance(), autoPauseEnabled);

	// update visual representation
	OptionsScreenState_updateAutomaticPauseCheckBox(this);
}

void OptionsScreenState_processUserInputModeShowOptions(OptionsScreenState this, UserInput userInput)
{
	if((K_A & userInput.pressedKey) || (K_STA & userInput.pressedKey) || (K_B & userInput.pressedKey) || (K_SEL & userInput.pressedKey))
	{
		// disable user input
		Game_disableKeypad(Game_getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity_playAnimation(transitionLayerEntity, "FadeOut");
		}
	}
	else if((K_LU & userInput.pressedKey) || (K_RU & userInput.pressedKey))
	{
		this->option = (this->option > 0) ? this->option - 1 : kOptionScreenOptionBrightness;
		OptionsScreenState_updateCursorPosition(this);
	}
	else if((K_LD & userInput.pressedKey) || (K_RD & userInput.pressedKey))
	{
		this->option = (this->option < kOptionScreenOptionBrightness) ? this->option + 1 : 0;
		OptionsScreenState_updateCursorPosition(this);
	}
	else if((K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey) || (K_LL & userInput.pressedKey) || (K_RL & userInput.pressedKey))
	{
		switch(this->option)
		{
			case kOptionScreenOptionLanguage:
				OptionsScreenState_switchLanguage(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;

			case kOptionScreenOptionAutomaticPause:
				OptionsScreenState_toggleAutomaticPause(this);
				break;

			case kOptionScreenOptionBrightness:
				OptionsScreenState_switchBrightness(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;
		}
	}
}

void OptionsScreenState_processUserInput(OptionsScreenState this, UserInput userInput)
{
	switch(this->mode)
	{
		case kOptionsScreenModeShowOptions:
		{
			OptionsScreenState_processUserInputModeShowOptions(this, userInput);
			break;
		}
	}
}

// handle event
static void OptionsScreenState_onTransitionOutComplete(OptionsScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "OptionsScreenState::onTransitionOutComplete: null this");

	// hide screen
	BrightnessManager_hideScreen(BrightnessManager_getInstance());

	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
}
