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
#include <stddef.h>

#include <Game.h>
#include <I18n.h>
#include <Error.h>
#include <GameEvents.h>
#include <ProgressManager.h>
#include <SRAMManager.h>
#include <Utilities.h>
#include <BrightnessManager.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void ProgressManager::constructor()
{
	// construct base object
	Base::constructor();
}

// class's destructor
void ProgressManager::destructor()
{
	// destroy base
	Base::destructor();
}

void ProgressManager::restoreSettings()
{
	Base::restoreSettings(this);

	// load and set brightness factor
	BrightnessManager::setBrightnessFactor(BrightnessManager::getInstance(), ProgressManager::getBrightnessFactor(this));
}

u8 ProgressManager::getBrightnessFactor()
{
	u8 brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct GameSaveData, brightnessFactor), sizeof(brightnessFactor));
	}

	return brightnessFactor;
}

void ProgressManager::setBrightnessFactor(u8 brightnessFactor)
{
	if(this->sramAvailable)
	{
		// write auto brightness factor
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct GameSaveData, brightnessFactor), sizeof(brightnessFactor));

		// write checksum
		SaveDataManager::writeChecksum(this);
	}
}
