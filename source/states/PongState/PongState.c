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
#include <Player.h>
#include <GameEvents.h>
#include <debugUtilities.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


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
static void PongState_onTransitionOutComplete(PongState this, Object eventFirer);


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

	// add event listeners
	Object_addEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)PongState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
static void PongState_destructor(PongState this)
{
	// remove event listeners
	Object_removeEventListener(__SAFE_CAST(Object, this), __SAFE_CAST(Object, this), (EventListener)PongState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
static void PongState_enter(PongState this, void* owner)
{
	// call base
	Base_enter(this, owner);

	// load stage
	GameState_loadStage(__SAFE_CAST(GameState, this), (StageDefinition*)&PLAYFIELD_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState_startClocks(__SAFE_CAST(GameState, this));

	// enable user input
	Game_enableKeypad(Game_getInstance());

	Player_getReady(Player_getInstance(), __SAFE_CAST(GameState, this));

	// show screen
	BrightnessManager_showScreen(BrightnessManager_getInstance());
}

// state's exit
static void PongState_exit(PongState this, void* owner)
{
	// call base
	Base_exit(this, owner);
}

// state's resume
static void PongState_resume(PongState this, void* owner)
{
	Base_resume(this, owner);

	Camera_startEffect(Camera_getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
static void PongState_suspend(PongState this, void* owner)
{
	Camera_startEffect(Camera_getInstance(), kFadeOut, __FADE_DELAY);

	Base_suspend(this, owner);
}

void PongState_processUserInput(PongState this, UserInput userInput)
{

	if(userInput.pressedKey & ~K_PWR)
	{
		if(userInput.pressedKey)
		{
			if(K_SEL & userInput.pressedKey)
			{
				// adjustment screen
//				PlatformerLevelState_setModeToPaused(this);

				// set next state of adjustment screen state to null so it can differentiate between
				// being called the splash screen sequence or from within the game (a bit hacky...)
//				SplashScreenState_setNextState(__SAFE_CAST(SplashScreenState, AdjustmentScreenState_getInstance()), NULL);

				// pause game and switch to adjustment screen state
//				Game_pause(Game_getInstance(), __SAFE_CAST(GameState, AdjustmentScreenState_getInstance()));

				return;
			}
			else if(K_STA & userInput.pressedKey)
			{
				// pause game and switch to pause screen state
//				Game_pause(Game_getInstance(), __SAFE_CAST(GameState, PauseScreenState_getInstance()));

				return;
			}
		}

	//	Object_fireEvent(__SAFE_CAST(Object, this), kEventUserInput);

	/*
		// disable user input
		Game_disableKeypad(Game_getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity_playAnimation(transitionLayerEntity, "FadeOut");
		}
	*/
	}

	Object_fireEvent(__SAFE_CAST(Object, this), kEventUserInput);

}

// handle event
static void PongState_onTransitionOutComplete(PongState this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	ASSERT(this, "PongState::onTransitionOutComplete: null this");

	// hide screen
	BrightnessManager_hideScreen(BrightnessManager_getInstance());

	Player_gameIsOver(Player_getInstance(), __SAFE_CAST(GameState, this));
}
