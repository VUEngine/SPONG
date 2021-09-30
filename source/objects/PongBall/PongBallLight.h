/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef PONG_BALL_LIGHT_H_
#define PONG_BALL_LIGHT_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Entity.h>
#include <PongBall.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											TYPE DEFINITIONS
//---------------------------------------------------------------------------------------------------------

typedef struct PongBallLightSpec
{
	// the base animated entity
	EntitySpec entitySpec;

} PongBallLightSpec;

typedef const PongBallLightSpec PongBallLightROMSpec;


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

class PongBallLight : Entity
{
	/* spec pointer */
	PongBallLightSpec* PongBallLightSpec;
	PongBall pongBall;
	fix10_6 pongBallInitialZDistance;
	bool followPongBall;

	void constructor(PongBallLightSpec* PongBallLightSpec, int16 id, int16 internalId, const char* const name);
	override void ready(bool recursive);
	override void update(uint32 elapsedTime);
	override bool handleMessage(Telegram telegram);
}


#endif
