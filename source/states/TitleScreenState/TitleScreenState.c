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
#include <TitleScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeyPadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <PongState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef TITLE_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void TitleScreenState_destructor(TitleScreenState this);
static void TitleScreenState_constructor(TitleScreenState this);
static void TitleScreenState_enter(TitleScreenState this, void* owner);
static void TitleScreenState_exit(TitleScreenState this, void* owner);
static void TitleScreenState_resume(TitleScreenState this, void* owner);
static void TitleScreenState_suspend(TitleScreenState this, void* owner);
void TitleScreenState_processUserInputModePressStart(TitleScreenState this, UserInput userInput);
void TitleScreenState_processUserInputModeShowOptions(TitleScreenState this, UserInput userInput);
void TitleScreenState_updateCursorPosition(TitleScreenState this);
static void TitleScreenState_onFadeInComplete(TitleScreenState this, Object eventFirer);
static void TitleScreenState_onFadeOutComplete(TitleScreenState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(TitleScreenState, GameState);
__SINGLETON(TitleScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) TitleScreenState_constructor(TitleScreenState this)
{
	__CONSTRUCT_BASE(GameState);

	// init members
	this->entityPressStart = NULL;
	this->entityMainMenu = NULL;
	this->entityCursor = NULL;
	this->mode = kTitleScreenModeShowPressStart;
	this->option = 0;
}

// class's destructor
static void TitleScreenState_destructor(TitleScreenState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void TitleScreenState_enter(TitleScreenState this, void* owner)
{
	// reset mode
	this->mode = kTitleScreenModeShowPressStart;

	// call base
	__CALL_BASE_METHOD(GameState, enter, this, owner);

	// disable user input
	Game_disableKeypad(Game_getInstance());

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&TITLE_SCREEN_STAGE_ST, NULL, true);

	// get entity references
	this->entityPressStart = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "PrssStrt", true));
	this->entityMainMenu = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "MainMenu", true));
	this->entityCursor = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "MMCursor", true));

	// initial entity states
	Entity_hide(__SAFE_CAST(Entity, this->entityMainMenu));
	TitleScreenState_updateCursorPosition(this);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// fade in screen
	Camera_startEffect(Camera_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))TitleScreenState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

// state's exit
static void TitleScreenState_exit(TitleScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, exit, this, owner);
}

// state's resume
static void TitleScreenState_resume(TitleScreenState this, void* owner)
{
	__CALL_BASE_METHOD(GameState, resume, this, owner);

	Camera_startEffect(Camera_getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
static void TitleScreenState_suspend(TitleScreenState this, void* owner)
{
	Camera_startEffect(Camera_getInstance(), kFadeOut, __FADE_DELAY);

	__CALL_BASE_METHOD(GameState, suspend, this, owner);
}

void TitleScreenState_updateCursorPosition(TitleScreenState this)
{
	s8 projectionCorrection = (this->option < 2) ? 1 : 0;
	Vector3D position =
	{
		0,
		__PIXELS_TO_METERS((this->option * 12) - 24 + projectionCorrection),
		__PIXELS_TO_METERS((this->option * -12) + 32),
	};
	Entity_setLocalPosition(this->entityCursor, &position);
}

void TitleScreenState_processUserInputModePressStart(TitleScreenState this, UserInput userInput)
{
	if(K_STA & userInput.pressedKey)
	{
		// remove blinking "press start button"
		Entity_hide(this->entityPressStart);

		// show options
		Entity_show(this->entityMainMenu);

		// set mode to showing options
		this->mode = kTitleScreenModeShowOptions;
	}
}

void TitleScreenState_processUserInputModeShowOptions(TitleScreenState this, UserInput userInput)
{
	if((K_A & userInput.pressedKey) || (K_STA & userInput.pressedKey))
	{
		// disable user input
		Game_disableKeypad(Game_getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity_playAnimation(transitionLayerEntity, "FadeOut");
		}

		// fade out screen
		Brightness brightness = (Brightness){0, 0, 0};
		Camera_startEffect(Camera_getInstance(),
			kFadeTo, // effect type
			500, // initial delay (in ms)
			&brightness, // target brightness
			__FADE_DELAY, // delay between fading steps (in ms)
			(void (*)(Object, Object))TitleScreenState_onFadeOutComplete, // callback function
			__SAFE_CAST(Object, this) // callback scope
		);
	}
	else if((K_LU & userInput.pressedKey) || (K_RU & userInput.pressedKey))
	{
		this->option = (this->option > 0) ? this->option - 1 : kTitleScreenOptionOptions;
		TitleScreenState_updateCursorPosition(this);
	}
	else if((K_LD & userInput.pressedKey) || (K_RD & userInput.pressedKey))
	{
		this->option = (this->option < kTitleScreenOptionOptions) ? this->option + 1 : 0;
		TitleScreenState_updateCursorPosition(this);
	}
}

void TitleScreenState_processUserInput(TitleScreenState this, UserInput userInput)
{
	switch(this->mode)
	{
		case kTitleScreenModeShowPressStart:
		{
			TitleScreenState_processUserInputModePressStart(this, userInput);
			break;
		}
		case kTitleScreenModeShowOptions:
		{
			TitleScreenState_processUserInputModeShowOptions(this, userInput);
			break;
		}
	}
}

// handle event
static void TitleScreenState_onFadeInComplete(TitleScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::onFadeInComplete: null this");

	// enable user input
	Game_enableKeypad(Game_getInstance());
}

// handle event
static void TitleScreenState_onFadeOutComplete(TitleScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "TitleScreenState::onFadeOutComplete: null this");

	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, PongState_getInstance()));
}
