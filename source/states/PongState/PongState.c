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
#include <PongState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeyPadManager.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef PLAYFIELD_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void PongState_destructor(PongState this);
static void PongState_constructor(PongState this);
static void PongState_enter(PongState this, void* owner);
static void PongState_exit(PongState this, void* owner);
static void PongState_resume(PongState this, void* owner);
static void PongState_suspend(PongState this, void* owner);
static void PongState_onFadeInComplete(PongState this, Object eventFirer);
static void PongState_onFadeOutComplete(PongState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(PongState, GameState);
__SINGLETON(PongState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) PongState_constructor(PongState this)
{
	__CONSTRUCT_BASE(GameState);
}

// class's destructor
static void PongState_destructor(PongState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void PongState_enter(PongState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, enter, this, owner);

	// disable user input
	Game_disableKeypad(Game_getInstance());

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&PLAYFIELD_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// fade in screen
	Camera_startEffect(Camera_getInstance(),
		kFadeTo, // effect type
		0, // initial delay (in ms)
		NULL, // target brightness
		__FADE_DELAY, // delay between fading steps (in ms)
		(void (*)(Object, Object))PongState_onFadeInComplete, // callback function
		__SAFE_CAST(Object, this) // callback scope
	);
}

// state's exit
static void PongState_exit(PongState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, exit, this, owner);
}

// state's resume
static void PongState_resume(PongState this, void* owner)
{
	__CALL_BASE_METHOD(GameState, resume, this, owner);

	Camera_startEffect(Camera_getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
static void PongState_suspend(PongState this, void* owner)
{
	Camera_startEffect(Camera_getInstance(), kFadeOut, __FADE_DELAY);

	__CALL_BASE_METHOD(GameState, suspend, this, owner);
}

void PongState_processUserInput(PongState this, UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
	{
	/*
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
			(void (*)(Object, Object))PongState_onFadeOutComplete, // callback function
			__SAFE_CAST(Object, this) // callback scope
		);
	*/
	}
}

// handle event
static void PongState_onFadeInComplete(PongState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "PongState::onFadeInComplete: null this");

	// enable user input
	Game_enableKeypad(Game_getInstance());
}

// handle event
static void PongState_onFadeOutComplete(PongState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "PongState::onFadeOutComplete: null this");
}
