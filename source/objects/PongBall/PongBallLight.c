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
#include "PongBallLight.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define Z_SCALING_COMPENSATION				__I_TO_FIX10_6(3)
#define WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING		150

//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------
//												CLASS'S PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void PongBallLight::onPongBallHitFloor(PongBallLight this, Object eventFirer __attribute__ ((unused)));
static void PongBallLight::onPongBallHitCeiling(PongBallLight this, Object eventFirer __attribute__ ((unused)));


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other



// class's constructor
void PongBallLight::constructor(PongBallLight this, PongBallLightDefinition* PongBallLightDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "PongBallLight::constructor: null this");

	// construct base
	Base::constructor((EntityDefinition*)&PongBallLightDefinition->entityDefinition, id, internalId, name);

	// save definition
	this->pongBall = NULL;
	this->pongBallInitialZDistance = __I_TO_FIX10_6(1);
	this->followPongBall = false;
}

// class's constructor
void PongBallLight::destructor(PongBallLight this)
{
	ASSERT(this, "PongBallLight::destructor: null this");

	Object::removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallLight_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::removeEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallLight_onPongBallHitCeiling, kEventPongBallHitCeiling);
	this->pongBall = NULL;

	// delete the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void PongBallLight::ready(PongBallLight this, bool recursive)
{
	ASSERT(this, "PongBallLight::ready: null this");

	// call base
	Base::ready(this, recursive);

	this->pongBall = __SAFE_CAST(PongBall, Container::getChildByName(__SAFE_CAST(Container, Game::getStage(Game::getInstance())), (char*)PONG_BALL_NAME, true));
	NM_ASSERT(this->pongBall, "PongBallLight::ready: null pongBall");
	this->pongBallInitialZDistance = this->transformation.globalPosition.z -  SpatialObject::getPosition(this->pongBall)->z;
	this->followPongBall = true;
	Object::addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallLight_onPongBallHitFloor, kEventPongBallHitFloor);
	Object::addEventListener(__SAFE_CAST(Object, this->pongBall), __SAFE_CAST(Object, this), (EventListener)PongBallLight_onPongBallHitCeiling, kEventPongBallHitCeiling);
}

void PongBallLight::update(PongBallLight this, u32 elapsedTime)
{
	Base::update(this, elapsedTime);

	if(this->followPongBall)
	{
		const Vector3D* pongBallPosition =  SpatialObject::getPosition(this->pongBall);

		Vector3D localPosition = this->transformation.localPosition;

		localPosition.x  = pongBallPosition->x;
		localPosition.y  = pongBallPosition->y;

		 Entity::setLocalPosition(this, &localPosition);

		Scale localScale = this->transformation.localScale;
		localScale.x = localScale.y = __FIX10_6_TO_FIX7_9(__FIX10_6_MULT(__I_TO_FIX10_6(1), __FIX10_6_DIV(this->pongBallInitialZDistance, this->transformation.globalPosition.z - (pongBallPosition->z - Z_SCALING_COMPENSATION))));
		Container::setLocalScale(__SAFE_CAST(Container, this), &localScale);
	}
}

static void PongBallLight::onPongBallHitFloor(PongBallLight this, Object eventFirer __attribute__ ((unused)))
{
	this->followPongBall = false;
	MessageDispatcher::dispatchMessage(WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kFollowPongBall, NULL);
}

static void PongBallLight::onPongBallHitCeiling(PongBallLight this, Object eventFirer __attribute__ ((unused)))
{
	this->followPongBall = false;
	MessageDispatcher::dispatchMessage(WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING, __SAFE_CAST(Object, this), __SAFE_CAST(Object, this), kFollowPongBall, NULL);
/*
	PixelVector displacement = Sprite::getDisplacement(VirtualList::front(this->sprites));
	displacement.parallax = -5;
	Sprite::setDisplacement(VirtualList::front(this->sprites), displacement);

	Scale localScale = this->transformation.localScale;
	localScale.x = localScale.y = __FIX10_6_TO_FIX7_9(__FIX10_6_MULT(__I_TO_FIX10_6(1), __I_TO_FIX10_6(2)));
	Container::setLocalScale(__SAFE_CAST(Container, this), &localScale);
	*/
}

bool PongBallLight::handleMessage(PongBallLight this, Telegram telegram)
{
	// process message
	switch(Telegram::getMessage(telegram))
	{
		case kFollowPongBall:
			{
				PixelVector displacement = Sprite::getDisplacement(VirtualList::front(this->sprites));
				displacement.parallax = 0;
				Sprite::setDisplacement(VirtualList::front(this->sprites), displacement);

				this->followPongBall = true;
			}

			return true;
			break;
	}

	return false;
}