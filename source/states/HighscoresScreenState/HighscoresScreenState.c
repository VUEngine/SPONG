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
#include <HighscoresScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeyPadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <TitleScreenState.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef HIGHSCORES_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void HighscoresScreenState_destructor(HighscoresScreenState this);
static void HighscoresScreenState_constructor(HighscoresScreenState this);
static void HighscoresScreenState_enter(HighscoresScreenState this, void* owner);
static void HighscoresScreenState_exit(HighscoresScreenState this, void* owner);
static void HighscoresScreenState_resume(HighscoresScreenState this, void* owner);
static void HighscoresScreenState_suspend(HighscoresScreenState this, void* owner);
static void HighscoresScreenState_onTransitionOutComplete(HighscoresScreenState this, Object eventFirer);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(HighscoresScreenState, GameState);
__SINGLETON(HighscoresScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
static void __attribute__ ((noinline)) HighscoresScreenState_constructor(HighscoresScreenState this)
{
	__CONSTRUCT_BASE(GameState);

	// add event listeners
	Object_addEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)HighscoresScreenState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
static void HighscoresScreenState_destructor(HighscoresScreenState this)
{
	// remove event listeners
	Object_removeEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)HighscoresScreenState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void HighscoresScreenState_enter(HighscoresScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, enter, this, owner);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&HIGHSCORES_SCREEN_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// enable user input
	Game_enableKeypad(Game_getInstance());

	// show screen
	Camera_startEffect(Camera_getInstance(), kShow);


 	/**/
	Printing_text(Printing_getInstance(), " 1   1,000,000   JORGE   ", 12,  1+13, NULL);
	Printing_text(Printing_getInstance(), " 2     500,000   CHRIS   ", 12,  2+13, NULL);
	Printing_text(Printing_getInstance(), " 3     100,000   SOMEONE ", 12,  3+13, NULL);
	Printing_text(Printing_getInstance(), " 4           -   -       ", 12,  4+13, NULL);
	Printing_text(Printing_getInstance(), " 5           -   -       ", 12,  5+13, NULL);
	Printing_text(Printing_getInstance(), " 6           -   -       ", 12,  6+13, NULL);
	Printing_text(Printing_getInstance(), " 7           -   -       ", 12,  7+13, NULL);
	Printing_text(Printing_getInstance(), " 8           -   -       ", 12,  8+13, NULL);
	Printing_text(Printing_getInstance(), " 9           -   -       ", 12,  9+13, NULL);
	Printing_text(Printing_getInstance(), "10           -   -       ", 12, 10+13, NULL);
	/**/
}

// state's exit
static void HighscoresScreenState_exit(HighscoresScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(GameState, exit, this, owner);
}

// state's resume
static void HighscoresScreenState_resume(HighscoresScreenState this, void* owner)
{
	__CALL_BASE_METHOD(GameState, resume, this, owner);

	Camera_startEffect(Camera_getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
static void HighscoresScreenState_suspend(HighscoresScreenState this, void* owner)
{
	Camera_startEffect(Camera_getInstance(), kFadeOut, __FADE_DELAY);

	__CALL_BASE_METHOD(GameState, suspend, this, owner);
}

void HighscoresScreenState_processUserInput(HighscoresScreenState this, UserInput userInput)
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

// handle event
static void HighscoresScreenState_onTransitionOutComplete(HighscoresScreenState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "HighscoresScreenState::onTransitionOutComplete: null this");

	Game_changeState(Game_getInstance(), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
}
