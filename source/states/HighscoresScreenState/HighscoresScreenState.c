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

#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <Languages.h>
#include <PhysicalWorld.h>
#include <HighscoresScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeypadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <TitleScreenState.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec HIGHSCORES_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void HighscoresScreenState::constructor()
{
	Base::constructor();
}

void HighscoresScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void HighscoresScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&HIGHSCORES_SCREEN_STAGE_ST, NULL, true, false);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());


 	/*/
	Printing::text(Printing::getInstance(), " 1   1,000,000   JORGE   ", 12,  1+13, NULL);
	Printing::text(Printing::getInstance(), " 2	 500,000   CHRIS   ", 12,  2+13, NULL);
	Printing::text(Printing::getInstance(), " 3	 100,000   SOMEONE ", 12,  3+13, NULL);
	Printing::text(Printing::getInstance(), " 4		   -   -	   ", 12,  4+13, NULL);
	Printing::text(Printing::getInstance(), " 5		   -   -	   ", 12,  5+13, NULL);
	Printing::text(Printing::getInstance(), " 6		   -   -	   ", 12,  6+13, NULL);
	Printing::text(Printing::getInstance(), " 7		   -   -	   ", 12,  7+13, NULL);
	Printing::text(Printing::getInstance(), " 8		   -   -	   ", 12,  8+13, NULL);
	Printing::text(Printing::getInstance(), " 9		   -   -	   ", 12,  9+13, NULL);
	Printing::text(Printing::getInstance(), "10		   -   -	   ", 12, 10+13, NULL);
	*/
}

void HighscoresScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

void HighscoresScreenState::processUserInput(UserInput userInput __attribute__ ((unused)))
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

void HighscoresScreenState::switchState()
{
	Base::switchState(this);

	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
