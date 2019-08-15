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
#include <VirtualList.h>
#include <CameraEffectManager.h>
#include <VIPManager.h>
#include <MessageDispatcher.h>
#include <GameEvents.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void BrightnessManager::constructor()
{
	// construct base object
	Base::constructor();

	// init members
	this->brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
}

void BrightnessManager::destructor()
{
	// destroy base
	Base::destructor();
}

void BrightnessManager::delayedShowScreen()
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	// show screen in a moment
	MessageDispatcher::dispatchMessage(50, Object::safeCast(this), Object::safeCast(this), kMessageShowScreen, NULL);
}

void BrightnessManager::showScreen()
{
	BrightnessManager::setDefaultBrightness(this);
	Object::fireEvent(Game::getCurrentState(Game::getInstance()), kEventShowScreen);
}

void BrightnessManager::hideScreen()
{
	__SET_BRIGHT(0, 0, 0);
}

void BrightnessManager::setDefaultBrightness()
{
	Brightness defaultBrightness = CameraEffectManager::getDefaultBrightness(CameraEffectManager::getInstance());

	int darkRed = (0 >= (darkRed = (defaultBrightness.darkRed + (this->brightnessFactor * 8)))) ? 4 : darkRed;
	int mediumRed = (0 >= (mediumRed = (defaultBrightness.mediumRed + (this->brightnessFactor * 16)))) ? 8 : mediumRed;
	int brightRed = (0 >= (brightRed = (defaultBrightness.brightRed + (this->brightnessFactor * 32)))) ? 16 : brightRed;

	__SET_BRIGHT(darkRed, mediumRed, brightRed);
}

void BrightnessManager::setBrightnessFactor(s8 brightnessFactor)
{
	this->brightnessFactor = brightnessFactor;
	BrightnessManager::setDefaultBrightness(this);
}

s8 BrightnessManager::getBrightnessFactor()
{
	return this->brightnessFactor;
}

bool BrightnessManager::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMessageShowScreen:
			// show screen
			BrightnessManager::showScreen(this);
			break;
	}
	return true;
}

