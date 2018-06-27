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

#include <BrightnessManager.h>
#include <VirtualList.h>
#include <CameraEffectManager.h>
#include <VIPManager.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void BrightnessManager::constructor()
{
	// construct base object
	Base::constructor();

	// init members
	this->brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
}

// class's destructor
void BrightnessManager::destructor()
{
	// destroy base
	Base::destructor();
}

void BrightnessManager::showScreen()
{
	Brightness defaultBrightness = CameraEffectManager::getDefaultBrightness(CameraEffectManager::getInstance());

	int darkRed = (0 >= (darkRed = (defaultBrightness.darkRed + ((this->brightnessFactor - 2) * 8)))) ? 4 : darkRed;
	int mediumRed = (0 >= (mediumRed = (defaultBrightness.mediumRed + ((this->brightnessFactor - 2) * 16)))) ? 8 : mediumRed;
	int brightRed = (0 >= (brightRed = (defaultBrightness.brightRed + ((this->brightnessFactor - 2) * 32)))) ? 16 : brightRed;

	__SET_BRIGHT(darkRed, mediumRed, brightRed);
}

void BrightnessManager::hideScreen()
{
	__SET_BRIGHT(0, 0, 0);
}

void BrightnessManager::setBrightnessFactor(u8 brightnessFactor)
{
	this->brightnessFactor = brightnessFactor;
	BrightnessManager::showScreen(this);
}

u8 BrightnessManager::getBrightnessFactor()
{
	return this->brightnessFactor;
}
