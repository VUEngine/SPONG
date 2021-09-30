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

void ProgressManager::constructor()
{
	// construct base object
	Base::constructor();
}

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

uint8 ProgressManager::getBrightnessFactor()
{
	int8 brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct GameSaveData, brightnessFactor), sizeof(brightnessFactor));
	}

	return brightnessFactor;
}

void ProgressManager::setBrightnessFactor(int8 brightnessFactor)
{
	if(this->sramAvailable)
	{
		// write auto brightness factor
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&brightnessFactor, offsetof(struct GameSaveData, brightnessFactor), sizeof(brightnessFactor));

		// write checksum
		SaveDataManager::writeChecksum(this);
	}
}

uint8 ProgressManager::getPlayerNumber()
{
	uint8 playerNumber = 1;
	if(this->sramAvailable)
	{
		SRAMManager::read(SRAMManager::getInstance(), (BYTE*)&playerNumber, offsetof(struct GameSaveData, playerNumber), sizeof(playerNumber));
	}

	return playerNumber;
}

void ProgressManager::setPlayerNumber(uint8 playerNumber)
{
	if(this->sramAvailable)
	{
		// write auto brightness factor
		SRAMManager::save(SRAMManager::getInstance(), (BYTE*)&playerNumber, offsetof(struct GameSaveData, playerNumber), sizeof(playerNumber));

		// write checksum
		SaveDataManager::writeChecksum(this);
	}
}
