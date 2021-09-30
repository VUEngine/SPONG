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

#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <KeypadManager.h>
#include <I18n.h>
#include <SoundManager.h>
#include <ProgressManager.h>
#include <PrecautionScreenState.h>
#include <AdjustmentScreenState.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <Languages.h>
#include <debugUtilities.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec PRECAUTION_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void PrecautionScreenState::constructor()
{
	Base::constructor();

	SplashScreenState::setNextState(SplashScreenState::safeCast(this), GameState::safeCast(AdjustmentScreenState::getInstance()));
	this->stageSpec = (StageSpec*)&PRECAUTION_SCREEN_STAGE_ST;
}

void PrecautionScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

// state's handle message
bool PrecautionScreenState::processMessage(void* owner __attribute__ ((unused)), Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kScreenStarted:
			{
				// play start-up sound
				//Vector3D position = {0, 0, 0};
				//SoundManager::playFxSound(SoundManager::getInstance(), COLLECT_SND, position);

				// show this screen for at least 2 seconds, as defined by Nintendo in the official development manual (Appendix 1)
				MessageDispatcher::dispatchMessage(2000, Object::safeCast(this), Object::safeCast(Game::getInstance()), kScreenAllowUserInput, NULL);

				// call base class' method
				Base::processMessage(this, owner, telegram);

				// make sure that keypad is not yet enabled
				Game::disableKeypad(Game::getInstance());
			}
			break;

		case kScreenAllowUserInput:
			{
				Game::enableKeypad(Game::getInstance());
			}
			break;
	}

	return false;
}

void PrecautionScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);
}
