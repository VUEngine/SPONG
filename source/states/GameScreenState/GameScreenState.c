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
#include <BrightnessManager.h>
#include <GameEvents.h>
#include <GameScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void GameScreenState::constructor()
{
	Base::constructor();

	// add event listeners
	Object::addEventListener(this, Object::safeCast(this), (EventListener)GameScreenState::onTransitionInComplete, kEventTransitionInComplete);
	Object::addEventListener(this, Object::safeCast(this), (EventListener)GameScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

void GameScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)GameScreenState::onTransitionInComplete, kEventTransitionInComplete);
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)GameScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	Base::destructor();
}

void GameScreenState::resume(void* owner)
{
	Base::resume(this, owner);

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void GameScreenState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

void GameScreenState::switchState()
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());
}

void GameScreenState::onTransitionInComplete(Object eventFirer __attribute__ ((unused)))
{
	// enable user input
	Game::enableKeypad(Game::getInstance());
}


void GameScreenState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	GameScreenState::switchState(this);
}
