/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef BRIGHTNESS_MANAGER_H_
#define BRIGHTNESS_MANAGER_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Object.h>


//---------------------------------------------------------------------------------------------------------
//												MACROS
//---------------------------------------------------------------------------------------------------------

#define DEFAULT_BRIGHTNESS_FACTOR		0


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

singleton class BrightnessManager : ListenerObject
{
	int8 brightnessFactor;

	static BrightnessManager getInstance();
	void showScreen();
	void delayedShowScreen();
	void hideScreen();
	void setBrightnessFactor(int8 brightnessFactor);
	int8 getBrightnessFactor();
	override bool handleMessage(Telegram telegram);
}


#endif
