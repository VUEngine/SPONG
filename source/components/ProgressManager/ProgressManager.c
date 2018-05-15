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
#include <AutoPauseScreenState.h>
#include <BrightnessManager.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

void ProgressManager::constructor(ProgressManager this);
bool ProgressManager::verifySaveStamp(ProgressManager this);
u32 ProgressManager::computeChecksum(ProgressManager this);
void ProgressManager::writeChecksum(ProgressManager this);
bool ProgressManager::verifyChecksum(ProgressManager this);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// it's a singleton


// class's constructor
void __attribute__ ((noinline)) ProgressManager::constructor(ProgressManager this)
{
	ASSERT(this, "ProgressManager::constructor: null this");

	// construct base object
	Base::constructor();

	// init class variables
	this->sramAvailable = false;
}

// class's destructor
void ProgressManager::destructor(ProgressManager this)
{
	ASSERT(this, "ProgressManager::destructor: null this");

	// destroy base
	__SINGLETON_DESTROY;
}

// write then immediately read save stamp to validate sram
bool ProgressManager::verifySaveStamp(ProgressManager this __attribute__ ((unused)))
{
	char saveStamp[SAVE_STAMP_LENGTH];

	// write save stamp
	SRAMManager::save(SRAMManager::getInstance(), (BYTE*)SAVE_STAMP, offsetof(struct SaveData, saveStamp), sizeof(saveStamp));

	// read save stamp
	SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&saveStamp, offsetof(struct SaveData, saveStamp), sizeof(saveStamp));

	return !strncmp(saveStamp, SAVE_STAMP, SAVE_STAMP_LENGTH);
}

u32 ProgressManager::computeChecksum(ProgressManager this __attribute__ ((unused)))
{
	u32 crc32 = ~0;

	// iterate over whole save data, starting right after the previously saved checksum
	int i = (offsetof(struct SaveData, checksum) + sizeof(crc32));
	for(; i < (int)sizeof(SaveData); i++)
	{
		// get the current byte
		u8 currentByte;
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&currentByte, i, sizeof(currentByte));

		// loop over all bits of the current byte and add to checksum
		u8 bit = 0;
		for(; bit < sizeof(currentByte); bit++)
		{
			if((crc32 & 1) != GET_BIT(currentByte, bit))
			{
				crc32 = (crc32 >> 1) ^ 0xEDB88320;
			}
			else
			{
				crc32 = (crc32 >> 1);
			}
		}
	}

	return ~crc32;
}

void ProgressManager::writeChecksum(ProgressManager this)
{
	u32 checksum = ProgressManager::computeChecksum(this);
	SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&checksum, offsetof(struct SaveData, checksum), sizeof(checksum));
}

bool ProgressManager::verifyChecksum(ProgressManager this)
{
	u32 computedChecksum = ProgressManager::computeChecksum(this);
	u32 savedChecksum = 0;
	SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&savedChecksum, offsetof(struct SaveData, checksum), sizeof(savedChecksum));

	return (computedChecksum == savedChecksum);
}

void ProgressManager::initialize(ProgressManager this)
{
	ASSERT(this, "ProgressManager::initialize: null this");

	// verify sram validity
	if(ProgressManager::verifySaveStamp(this))
	{
		// set sram available flag
		this->sramAvailable = true;

		// verify saved progress presence and integrity
		if(!ProgressManager::verifyChecksum(this))
		{
			// if no previous save could be verified, completely erase sram to start clean
			SRAMManager::clear(SRAMManager::getInstance(), 0, (int)sizeof(SaveData));

			ProgressManager::setBrightnessFactor(this, DEFAULT_BRIGHTNESS_FACTOR);

			// write checksum
			ProgressManager::writeChecksum(this);
		}

		// load and set active language
		I18n::setActiveLanguage(I18n::getInstance(), ProgressManager::getLanguage(this));

		// load and set auto pause state
		Game::setAutomaticPauseState(Game::getInstance(), ProgressManager::getAutomaticPauseStatus(this)
			? __SAFE_CAST(GameState, AutoPauseScreenState::getInstance())
			: NULL
		);

		// load and set brightness factor
		BrightnessManager::setBrightnessFactor(BrightnessManager::getInstance(), ProgressManager::getBrightnessFactor(this));
	}
}

u8 ProgressManager::getLanguage(ProgressManager this)
{
	ASSERT(this, "ProgressManager::getLanguage: null this");

	u8 languageId = 0;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&languageId, offsetof(struct SaveData, languageId), sizeof(languageId));
	}

	return languageId;
}

void ProgressManager::setLanguage(ProgressManager this, u8 languageId)
{
	ASSERT(this, "ProgressManager::setLanguage: null this");

	if(this->sramAvailable)
	{
		// write language
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&languageId, offsetof(struct SaveData, languageId), sizeof(languageId));

		// write checksum
		ProgressManager::writeChecksum(this);
	}
}

bool ProgressManager::getAutomaticPauseStatus(ProgressManager this)
{
	ASSERT(this, "ProgressManager::getAutomaticPause: null this");

	u8 autoPauseStatus = 0;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&autoPauseStatus, offsetof(struct SaveData, autoPauseStatus), sizeof(autoPauseStatus));
	}

	return !autoPauseStatus;
}

void ProgressManager::setAutomaticPauseStatus(ProgressManager this, u8 autoPauseStatus)
{
	ASSERT(this, "ProgressManager::setAutomaticPause: null this");

	if(this->sramAvailable)
	{
		// we save the inverted status, so that 0 = enabled, 1 = disabled.
		// that way, a blank value means enabled, which is the standard setting.
		autoPauseStatus = !autoPauseStatus;

		// write auto pause status
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&autoPauseStatus, offsetof(struct SaveData, autoPauseStatus), sizeof(autoPauseStatus));

		// write checksum
		ProgressManager::writeChecksum(this);
	}
}

u8 ProgressManager::getBrightnessFactor(ProgressManager this)
{
	ASSERT(this, "ProgressManager::getBrightnessFactor: null this");

	u8 brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct SaveData, brightnessFactor), sizeof(brightnessFactor));
	}

	return brightnessFactor;
}

void ProgressManager::setBrightnessFactor(ProgressManager this, u8 brightnessFactor)
{
	ASSERT(this, "ProgressManager::setBrightnessFactor: null this");

	if(this->sramAvailable)
	{
		// write auto brightness factor
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct SaveData, brightnessFactor), sizeof(brightnessFactor));

		// write checksum
		ProgressManager::writeChecksum(this);
	}
}
