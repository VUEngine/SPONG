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

#include <string.h>

#include <VUEngine.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <Languages.h>
#include <PhysicalWorld.h>
#include <CreditsScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeypadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <TitleScreenState.h>
#include <AutomaticPauseManager.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec CreditsScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void CreditsScreenState::constructor()
{
	Base::constructor();
}

void CreditsScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void CreditsScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&CreditsScreenStage, NULL, true, false);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void CreditsScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

void CreditsScreenState::processUserInput(UserInput userInput)
{
	if((K_A | K_B | K_STA | K_SEL) & userInput.pressedKey)
	{
		// disable user input
		VUEngine::disableKeypad(VUEngine::getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
		}
	}
}

void CreditsScreenState::switchState()
{
	Base::switchState(this);

	VUEngine::changeState(VUEngine::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
