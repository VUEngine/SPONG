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
void OptionsScreenState_processUserInputModeShowOptions(OptionsScreenState this, UserInput userInput);
void OptionsScreenState_updateCursorPosition(OptionsScreenState this);
static void OptionsScreenState_onFadeInComplete(OptionsScreenState this, Object eventFirer);
static void OptionsScreenState_onFadeOutComplete(OptionsScreenState this, Object eventFirer);


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
}

// class's destructor
static void OptionsScreenState_destructor(OptionsScreenState this)
{
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

	// disable user input
	Game_disableKeypad(Game_getInstance());

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&OPTIONS_SCREEN_STAGE_ST, NULL, true);

	// get entity references
	//this->entityCursor = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "MMCursor", true));

	// initial entity states
	//OptionsScreenState_updateCursorPosition(this);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// fade in screen
	Camera_startEffect(Camera_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))OptionsScreenState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);

	/*/
	Printing_text(Printing_getInstance(), "TEST", 0, 0, NULL);
	Printing_text(Printing_getInstance(), "TEST", 0, 1, NULL);
	Printing_text(Printing_getInstance(), "TEST", 0, 2, NULL);
	Printing_text(Printing_getInstance(), "TEST", 0, 3, NULL);
	/**/
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
	s8 projectionCorrection = (this->option < 2) ? 1 : 0;
	Vector3D position =
	{
		0,
		__PIXELS_TO_METERS((this->option * 12) - 24 + projectionCorrection),
		__PIXELS_TO_METERS((this->option * -12) + 32),
	};
	Entity_setLocalPosition(this->entityCursor, &position);
}

void OptionsScreenState_processUserInputModeShowOptions(OptionsScreenState this, UserInput userInput)
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
			(void (*)(Object, Object))OptionsScreenState_onFadeOutComplete, // callback function
			__SAFE_CAST(Object, this) // callback scope
		);
	}
	else if((K_LU & userInput.pressedKey) || (K_RU & userInput.pressedKey))
	{
		this->option = (this->option > 0) ? this->option - 1 : kOptionScreenOption3;
		OptionsScreenState_updateCursorPosition(this);
	}
	else if((K_LD & userInput.pressedKey) || (K_RD & userInput.pressedKey))
	{
		this->option = (this->option < kOptionScreenOption3) ? this->option + 1 : 0;
		OptionsScreenState_updateCursorPosition(this);
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
static void OptionsScreenState_onFadeInComplete(OptionsScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "OptionsScreenState::onFadeInComplete: null this");

	// enable user input
	Game_enableKeypad(Game_getInstance());
}

// handle event
static void OptionsScreenState_onFadeOutComplete(OptionsScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "OptionsScreenState::onFadeOutComplete: null this");

	switch(this->option)
	{
		case kOptionScreenOption1:
			Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
			break;
	}
}
