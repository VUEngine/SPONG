/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef ADJUSTMENT_SCREEN_STATE_H_
#define ADJUSTMENT_SCREEN_STATE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <SplashScreenState.h>


//---------------------------------------------------------------------------------------------------------
//												MACROS
//---------------------------------------------------------------------------------------------------------

#define ADJUSTMENT_SCREEN_RHOMBUS_INITIAL_VALUE		-64


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

dynamic_singleton class AdjustmentScreenState : SplashScreenState
{
	static AdjustmentScreenState getInstance();
	override void enter(void* owner);
	override void processInput(uint32 releasedKey);
}


#endif
