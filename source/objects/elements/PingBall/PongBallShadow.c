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
#include <CollisionManager.h>
#include <Optics.h>
#include <CollisionManager.h>
#include <Utilities.h>
#include <GameEvents.h>
#include <MessageDispatcher.h>
#include <debugUtilities.h>
#include "PongBallShadow.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define Z_SCALING_COMPENSATION				__I_TO_FIX10_6(3)
#define WAIT_AFTER_PONG_BALL_HIT_FLOOR		150

//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(PongBallShadow, Entity);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void PongBallShadow_onPongBallHitFloor(PongBallShadow this, Object eventFirer __attribute__ ((unused)));


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other
__CLASS_NEW_DEFINITION(PongBallShadow, PongBallShadowDefinition* pongBallShadowDefinition, s16 id, s16 internalId, const char* const name)
__CLASS_NEW_END(PongBallShadow, pongBallShadowDefinition, id, internalId, name);

// class's constructor
void PongBallShadow_constructor(PongBallShadow this, PongBallShadowDefinition* pongBallShadowDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "PongBallShadow::constructor: null this");

	// construct base
	__CONSTRUCT_BASE(Entity, (EntityDefinition*)&pongBallShadowDefinition->entityDefinition, id, internalId, name);

	// save definition
	this->pongBall = NULL;
	this->pongBallInitialZDistance = __I_TO_FIX10_6(1);
	this->followPongBall = false;
}

// class's constructor
void PongBallShadow_destructor(PongBallShadow this)
{
	ASSERT(this, "PongBallShadow::destructor: null this");

	Object_removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallShadow_onPongBallHitFloor, kEventPongBallHitFloor);
	this->pongBall = NULL;

	// delete the super object
	// must always be called at the end of the destructor
	__DESTROY_BASE;
}

void PongBallShadow_ready(PongBallShadow this, bool recursive)
{
	ASSERT(this, "PongBallShadow::ready: null this");

	// call base
	__CALL_BASE_METHOD(Entity, ready, this, recursive);

	this->pongBall = __SAFE_CAST(PongBall, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), "PongBall", true));
	NM_ASSERT(this->pongBall, "PongBallShadow::ready: null pongBall");
	this->pongBallInitialZDistance = this->transformation.globalPosition.z - __VIRTUAL_CALL(SpatialObject, getPosition, this->pongBall)->z;
	this->followPongBall = true;
	Object_addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallShadow_onPongBallHitFloor, kEventPongBallHitFloor);
}

void PongBallShadow_update(PongBallShadow this, u32 elapsedTime)
{
	__CALL_BASE_METHOD(Entity, update , this, elapsedTime);

	if(this->followPongBall)
	{
		const Vector3D* pongBallPosition = __VIRTUAL_CALL(SpatialObject, getPosition, this->pongBall);

		Vector3D localPosition = this->transformation.localPosition;

		localPosition.x  = pongBallPosition->x;
		localPosition.y  = pongBallPosition->y;

		__VIRTUAL_CALL(Entity, setLocalPosition, this, &localPosition);

		Scale localScale = this->transformation.localScale;

		localScale.x = localScale.y = __FIX10_6_TO_FIX7_9(__FIX10_6_MULT(__I_TO_FIX10_6(1), __FIX10_6_DIV(this->pongBallInitialZDistance, this->transformation.globalPosition.z - (pongBallPosition->z - Z_SCALING_COMPENSATION))));

		Container_setLocalScale(__SAFE_CAST(Container, this), &localScale);
	}
}

static void PongBallShadow_onPongBallHitFloor(PongBallShadow this, Object eventFirer __attribute__ ((unused)))
{
	this->followPongBall = false;
	MessageDispatcher_dispatchMessage(WAIT_AFTER_PONG_BALL_HIT_FLOOR, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kFollowPongBall, NULL);
}

bool PongBallShadow_handleMessage(PongBallShadow this, Telegram telegram)
{
	// process message
	switch(Telegram_getMessage(telegram))
	{
		case kFollowPongBall:

			this->followPongBall = true;

			return true;
			break;
	}

	return false;
}