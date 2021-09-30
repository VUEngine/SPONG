/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef PROGRESS_MANAGER_H_
#define PROGRESS_MANAGER_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <SaveDataManager.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

// this struct is never instantiated, its sole purpose is to determine offsets of its members.
// therefore it acts as kind of like a map of sram content.
typedef struct GameSaveData
{
	// save data handled by base class
	SaveData baseSaveData;

	// brightness factor
	uint8 brightnessFactor;

	// Player
	uint8 playerNumber;

} GameSaveData;


//---------------------------------------------------------------------------------------------------------
//											TYPE DEFINITIONS
//---------------------------------------------------------------------------------------------------------

singleton class ProgressManager : SaveDataManager
{
	// declare class
	static ProgressManager getInstance();
	uint8   getBrightnessFactor();
	void setBrightnessFactor(int8 brightnessFactor);
	override void restoreSettings();
	uint8 getPlayerNumber();
	void setPlayerNumber(uint8 playerNumber);
}


#endif
