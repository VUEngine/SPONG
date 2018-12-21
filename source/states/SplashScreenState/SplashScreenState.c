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
#include <SplashScreenState.h>
#include <KeypadManager.h>
#include <AnimatedEntity.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void SplashScreenState::constructor()
{
	Base::constructor();

	this->stageSpec = NULL;
}

void SplashScreenState::destructor()
{
	// destroy the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void SplashScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	if(this->stageSpec)
	{
		GameState::loadStage(GameState::safeCast(this), this->stageSpec, NULL, true);
	}

	SplashScreenState::print(this);

	// start fade in effect in 1 ms, because a full render cycle is needed to put everything in place
	MessageDispatcher::dispatchMessage(1, Object::safeCast(this), Object::safeCast(Game::getInstance()), kScreenStarted, NULL);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void SplashScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);

	// destroy the state
	delete this;
}

// state's resume
void SplashScreenState::resume(void* owner)
{
	Base::resume(this, owner);

	SplashScreenState::print(this);
}

void SplashScreenState::processUserInput(UserInput userInput)
{
	SplashScreenState::processInput(this, userInput.pressedKey);
}

// state's handle message
bool SplashScreenState::processMessage(void* owner __attribute__ ((unused)), Telegram telegram __attribute__ ((unused)))
{
	return false;
}

void SplashScreenState::processInput(u32 pressedKey __attribute__ ((unused)))
{
	SplashScreenState::loadNextState(this);
}

void SplashScreenState::print()
{
}

void SplashScreenState::setNextState(GameState nextState)
{
	this->nextState = nextState;
}

void SplashScreenState::loadNextState()
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

void SplashScreenState::switchState()
{
	Base::switchState(this);

	// change to next stage
	Game::changeState(Game::getInstance(), this->nextState);
}
