/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <VUEngine.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <AdjustmentScreenState.h>
#include <LangSelectScreenState.h>
#include <DirectDraw.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec AdjustmentScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void AdjustmentScreenState::constructor()
{
	Base::constructor();

	SplashScreenState::setNextState(SplashScreenState::safeCast(this), GameState::safeCast(LangSelectScreenState::getInstance()));
	this->stageSpec = (StageSpec*)&AdjustmentScreenStage;
}

void AdjustmentScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void AdjustmentScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// move the printing area out of the visible screen to save CPU resources
	Printing::setWorldCoordinates(Printing::getInstance(), __SCREEN_WIDTH, __SCREEN_HEIGHT, 0, 0);
}

void AdjustmentScreenState::processInput(uint32 pressedKey __attribute__ ((unused)))
{
	// TODO: replace this ugly hack with a proper Game_isPaused check or something similar
	if(this->nextState == NULL)
	{
		Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);
		VUEngine::unpause(VUEngine::getInstance(), GameState::safeCast(this));
	}
	else
	{
		SplashScreenState::loadNextState(this);
	}
}
