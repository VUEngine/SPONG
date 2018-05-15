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

#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <AdjustmentScreenState.h>
#include <LangSelectScreenState.h>
#include <DirectDraw.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef ADJUSTMENT_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

void AdjustmentScreenState::constructor(AdjustmentScreenState this);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------


__SINGLETON_DYNAMIC(AdjustmentScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void __attribute__ ((noinline)) AdjustmentScreenState::constructor(AdjustmentScreenState this)
{
	Base::constructor();

	SplashScreenState::setNextState(__SAFE_CAST(SplashScreenState, this), __SAFE_CAST(GameState, LangSelectScreenState::getInstance()));
	this->stageDefinition = (StageDefinition*)&ADJUSTMENT_SCREEN_STAGE_ST;
}

// class's destructor
void AdjustmentScreenState::destructor(AdjustmentScreenState this)
{
	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void AdjustmentScreenState::enter(AdjustmentScreenState this, void* owner)
{
	// call base
	Base::enter(this, owner);

	// move the printing area out of the visible screen to save CPU resources
	Printing::setWorldCoordinates(Printing::getInstance(), __SCREEN_WIDTH, __SCREEN_HEIGHT);
}

void AdjustmentScreenState::processInput(AdjustmentScreenState this, u32 pressedKey __attribute__ ((unused)))
{
	// TODO: replace this ugly hack with a proper Game_isPaused check or something similar
	if(this->nextState == NULL)
	{
		Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);
		Game::unpause(Game::getInstance(), __SAFE_CAST(GameState, this));
	}
	else
	{
		SplashScreenState::loadNextState(__SAFE_CAST(SplashScreenState, this));
	}
}
