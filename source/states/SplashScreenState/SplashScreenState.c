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
		GameState::loadStage(GameState::safeCast(this), this->stageSpec, NULL, true, false);
	}

	SplashScreenState::print(this);

	// start fade in effect in 1 ms, because a full render cycle is needed to put everything in place
	MessageDispatcher::dispatchMessage(1, ListenerObject::safeCast(this), ListenerObject::safeCast(VUEngine::getInstance()), kScreenStarted, NULL);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

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

void SplashScreenState::processInput(uint32 pressedKey __attribute__ ((unused)))
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
	VUEngine::disableKeypad(VUEngine::getInstance());

	// transition layer animation
	AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(VUEngine::getStage(VUEngine::getInstance())), "TRNSLYR", true));
	if(transitionLayerEntity)
	{
		AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
	}
}

void SplashScreenState::switchState()
{
	Base::switchState(this);

	// change to next stage
	VUEngine::changeState(VUEngine::getInstance(), this->nextState);
}
