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
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&HIGHSCORES_SCREEN_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());


 	/*/
	Printing::text(Printing::getInstance(), " 1   1,000,000   JORGE   ", 12,  1+13, NULL);
	Printing::text(Printing::getInstance(), " 2     500,000   CHRIS   ", 12,  2+13, NULL);
	Printing::text(Printing::getInstance(), " 3     100,000   SOMEONE ", 12,  3+13, NULL);
	Printing::text(Printing::getInstance(), " 4           -   -       ", 12,  4+13, NULL);
	Printing::text(Printing::getInstance(), " 5           -   -       ", 12,  5+13, NULL);
	Printing::text(Printing::getInstance(), " 6           -   -       ", 12,  6+13, NULL);
	Printing::text(Printing::getInstance(), " 7           -   -       ", 12,  7+13, NULL);
	Printing::text(Printing::getInstance(), " 8           -   -       ", 12,  8+13, NULL);
	Printing::text(Printing::getInstance(), " 9           -   -       ", 12,  9+13, NULL);
	Printing::text(Printing::getInstance(), "10           -   -       ", 12, 10+13, NULL);
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
