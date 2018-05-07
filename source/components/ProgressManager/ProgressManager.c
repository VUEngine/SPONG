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
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(ProgressManager, Object);


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void ProgressManager_constructor(ProgressManager this);
bool ProgressManager_verifySaveStamp(ProgressManager this);
u32 ProgressManager_computeChecksum(ProgressManager this);
void ProgressManager_writeChecksum(ProgressManager this);
bool ProgressManager_verifyChecksum(ProgressManager this);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// it's a singleton
__SINGLETON(ProgressManager);

// class's constructor
static void __attribute__ ((noinline)) ProgressManager_constructor(ProgressManager this)
{
	ASSERT(this, "ProgressManager::constructor: null this");

	// construct base object
	__CONSTRUCT_BASE(Object);

	// init class variables
	this->sramAvailable = false;
}

// class's destructor
void ProgressManager_destructor(ProgressManager this)
{
	ASSERT(this, "ProgressManager::destructor: null this");

	// destroy base
	__SINGLETON_DESTROY;
}

// write then immediately read save stamp to validate sram
bool ProgressManager_verifySaveStamp(ProgressManager this __attribute__ ((unused)))
{
	char saveStamp[SAVE_STAMP_LENGTH];

	// write save stamp
	SRAMManager_save(SRAMManager_getInstance(), (BYTE*)SAVE_STAMP, offsetof(struct SaveData, saveStamp), sizeof(saveStamp));

	// read save stamp
	SRAMManager_read(SRAMManager_getInstance(), (BYTE*)&saveStamp, offsetof(struct SaveData, saveStamp), sizeof(saveStamp));

	return !strncmp(saveStamp, SAVE_STAMP, SAVE_STAMP_LENGTH);
}

u32 ProgressManager_computeChecksum(ProgressManager this __attribute__ ((unused)))
{
	u32 crc32 = ~0;

	// iterate over whole save data, starting right after the previously saved checksum
	int i = (offsetof(struct SaveData, checksum) + sizeof(crc32));
	for(; i < (int)sizeof(SaveData); i++)
	{
		// get the current byte
		u8 currentByte;
		SRAMManager_read(SRAMManager_getInstance(), (BYTE*)&currentByte, i, sizeof(currentByte));

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

void ProgressManager_writeChecksum(ProgressManager this)
{
	u32 checksum = ProgressManager_computeChecksum(this);
	SRAMManager_save(SRAMManager_getInstance(), (BYTE*)&checksum, offsetof(struct SaveData, checksum), sizeof(checksum));
}

bool ProgressManager_verifyChecksum(ProgressManager this)
{
	u32 computedChecksum = ProgressManager_computeChecksum(this);
	u32 savedChecksum = 0;
	SRAMManager_read(SRAMManager_getInstance(), (BYTE*)&savedChecksum, offsetof(struct SaveData, checksum), sizeof(savedChecksum));

	return (computedChecksum == savedChecksum);
}

void ProgressManager_initialize(ProgressManager this)
{
	ASSERT(this, "ProgressManager::initialize: null this");

	// verify sram validity
	if(ProgressManager_verifySaveStamp(this))
	{
		// set sram available flag
		this->sramAvailable = true;

		// verify saved progress presence and integrity
		if(!ProgressManager_verifyChecksum(this))
		{
			// if no previous save could be verified, completely erase sram to start clean
			SRAMManager_clear(SRAMManager_getInstance(), 0, (int)sizeof(SaveData));

			// write checksum
			ProgressManager_writeChecksum(this);
		}

		// load and set active language
		I18n_setActiveLanguage(I18n_getInstance(), ProgressManager_getLanguage(this));

		// load and set auto pause state
		Game_setAutomaticPauseState(Game_getInstance(), ProgressManager_getAutomaticPauseStatus(this)
			? __SAFE_CAST(GameState, AutoPauseScreenState_getInstance())
			: NULL
		);
	}
}

u8 ProgressManager_getLanguage(ProgressManager this)
{
	ASSERT(this, "ProgressManager::getLanguage: null this");

	u8 languageId = 0;
	if(this->sramAvailable)
	{
		SRAMManager_read(SRAMManager_getInstance(), (BYTE*)&languageId, offsetof(struct SaveData, languageId), sizeof(languageId));
	}

	return languageId;
}

void ProgressManager_setLanguage(ProgressManager this, u8 languageId)
{
	ASSERT(this, "ProgressManager::setLanguage: null this");

	if(this->sramAvailable)
	{
		// write language
		SRAMManager_save(SRAMManager_getInstance(), (BYTE*)&languageId, offsetof(struct SaveData, languageId), sizeof(languageId));

		// write checksum
		ProgressManager_writeChecksum(this);
	}
}

bool ProgressManager_getAutomaticPauseStatus(ProgressManager this)
{
	ASSERT(this, "ProgressManager::getAutomaticPause: null this");

	u8 autoPauseStatus = 0;
	if(this->sramAvailable)
	{
		SRAMManager_read(SRAMManager_getInstance(), (BYTE*)&autoPauseStatus, offsetof(struct SaveData, autoPauseStatus), sizeof(autoPauseStatus));
	}

	return !autoPauseStatus;
}

void ProgressManager_setAutomaticPauseStatus(ProgressManager this, u8 autoPauseStatus)
{
	ASSERT(this, "ProgressManager::setAutomaticPause: null this");

	if(this->sramAvailable)
	{
		// we save the inverted status, so that 0 = enabled, 1 = disabled.
		// that way, a blank value means enabled, which is the standard setting.
		autoPauseStatus = !autoPauseStatus;

		// write auto pause status
		SRAMManager_save(SRAMManager_getInstance(), (BYTE*)&autoPauseStatus, offsetof(struct SaveData, autoPauseStatus), sizeof(autoPauseStatus));

		// write checksum
		ProgressManager_writeChecksum(this);
	}
}
