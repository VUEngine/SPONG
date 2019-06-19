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
