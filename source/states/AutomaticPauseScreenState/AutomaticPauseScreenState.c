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
#include <Printing.h>
#include <MessageDispatcher.h>
#include <AnimatedEntity.h>
#include <BrightnessManager.h>
#include <GameEvents.h>
#include <AutomaticPauseScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec AUTO_PauseScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void AutomaticPauseScreenState::constructor()
{
	Base::constructor();
}

void AutomaticPauseScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void AutomaticPauseScreenState::enter(void* owner __attribute__ ((unused)))
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(this, (StageSpec*)&AUTO_PauseScreenStage, NULL, true, false);

	// start clocks to start animations
	GameState::startClocks(this);

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void AutomaticPauseScreenState::exit(void* owner __attribute__ ((unused)))
{
	// call base
	Base::exit(this, owner);
}

void AutomaticPauseScreenState::processUserInput(UserInput userInput)
{
	if(K_STA & userInput.pressedKey)
	{
		// disable user input
		Game::disableKeypad(Game::getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
		}
	}
}

void AutomaticPauseScreenState::switchState()
{
	Base::switchState(this);

	// resume game
	Game::unpause(Game::getInstance(), GameState::safeCast(this));
}
