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
#include <SpongScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void SpongScreenState::constructor()
{
	Base::constructor();

	// add event listeners
	Object::addEventListener(this, Object::safeCast(this), (EventListener)SpongScreenState::onTransitionInComplete, kEventTransitionInComplete);
	Object::addEventListener(this, Object::safeCast(this), (EventListener)SpongScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

void SpongScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)SpongScreenState::onTransitionInComplete, kEventTransitionInComplete);
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)SpongScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	Base::destructor();
}

void SpongScreenState::resume(void* owner)
{
	Base::resume(this, owner);

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void SpongScreenState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

void SpongScreenState::switchState()
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());
}

void SpongScreenState::onTransitionInComplete(Object eventFirer __attribute__ ((unused)))
{
	// enable user input
	Game::enableKeypad(Game::getInstance());
}


void SpongScreenState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	SpongScreenState::switchState(this);
}
