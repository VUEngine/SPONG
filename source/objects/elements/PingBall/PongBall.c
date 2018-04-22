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
#include <MessageDispatcher.h>
#include <debugUtilities.h>
#include "PongBall.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(PongBall, Actor);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other
__CLASS_NEW_DEFINITION(PongBall, PongBallDefinition* PongBallDefinition, s16 id, s16 internalId, const char* const name)
__CLASS_NEW_END(PongBall, PongBallDefinition, id, internalId, name);

// class's constructor
void PongBall_constructor(PongBall this, PongBallDefinition* PongBallDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "PongBall::constructor: null this");

	// construct base
	__CONSTRUCT_BASE(Actor, (ActorDefinition*)&PongBallDefinition->actorDefinition, id, internalId, name);

	// save definition
	this->PongBallDefinition = PongBallDefinition;
}

// class's constructor
void PongBall_destructor(PongBall this)
{
	ASSERT(this, "PongBall::destructor: null this");

	// delete the super object
	// must always be called at the end of the destructor
	__DESTROY_BASE;
}

void PongBall_ready(PongBall this, bool recursive)
{
	ASSERT(this, "PongBall::ready: null this");

	// call base
	__CALL_BASE_METHOD(Actor, ready, this, recursive);

	PongBall_startMovement(this);
}

void PongBall_update(PongBall this, u32 elapsedTime)
{
	__CALL_BASE_METHOD(Actor, update , this, elapsedTime);

	//Shape_show(__SAFE_CAST(Shape, VirtualList_front(this->shapes)));
	Rotation localRotation = this->transformation.localRotation;
	localRotation.z += __FIX10_6_TO_I(Vector3D_squareLength(Body_getVelocity(this->body))) >> 4;
	Entity_setLocalRotation(this, &localRotation);
//	Body_print(this->body, 1, 1);
}

// start moving
void PongBall_startMovement(PongBall this)
{
	long seed = Utilities_randomSeed();
/*
	Force force =
	{
		__I_TO_FIX10_6(Utilities_random(seed, 10)),
		__I_TO_FIX10_6(Utilities_random(seed, 10)),
		0,
	};

	Actor_addForce(__SAFE_CAST(Actor, this), &force);*/
}

// move back to ejector
void PongBall_stopMovement(PongBall this)
{
	// stop movement
	Actor_stopAllMovement(__SAFE_CAST(Actor, this));
}

// state's handle message
bool PongBall_handleMessage(PongBall this, Telegram telegram)
{
	ASSERT(this, "PongBall::handleMessage: null this");

	switch(Telegram_getMessage(telegram))
	{
	}

	return Actor_handleMessage(__SAFE_CAST(Actor, this), telegram);
}

bool PongBall_enterCollision(PongBall this, const CollisionInformation* collisionInformation)
{

	ASSERT(this, "Hero::enterCollision: null this");
	ASSERT(collisionInformation->collidingShape, "Hero::enterCollision: null collidingObjects");

	Shape collidingShape = collisionInformation->collidingShape;
	SpatialObject collidingObject = Shape_getOwner(collidingShape);

	SolutionVector solutionVector = collisionInformation->solutionVector;
	Velocity velocity = Body_getVelocity(this->body);
	Vector3D direction = Vector3D_normalize(velocity);

	Force force =
	{
		0,
		0,
		__I_TO_FIX10_6(-100)
//			0 >= collisionInformation->solutionVector.direction.z ? __FIX10_6_MULT(__I_TO_FIX10_6(-100), (__1I_FIX10_6 - __ABS(collisionInformation->solutionVector.direction.z))) : 0
	};

	bool addForce = false;

	switch(__VIRTUAL_CALL(SpatialObject, getInGameType, collidingObject))
	{
		case kPaddleType:

			addForce = true;
			force.y = this->transformation.globalPosition.x - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->x;
			force.y += this->transformation.globalPosition.y - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->y;
			force.y = __FIX10_6_MULT(force.y, __I_TO_FIX10_6(-100));
			break;

		case kWall:
		{
			addForce = true;
			static int leftScore = 0;
			static int rightScore = 0;

			u32 collidingShapeLayers = Shape_getLayers(collidingShape);

			if(kPlayFieldLeftFloorLayer == collidingShapeLayers)
			{
				rightScore++;
				PRINT_INT(rightScore, 45, 0);
			}
			else if(kPlayFieldRightFloorLayer == collidingShapeLayers)
			{
				leftScore++;
				PRINT_INT(leftScore, 1, 0);
			}
		}

		break;
	}

	bool collisionResult = Actor_enterCollision(__SAFE_CAST(Actor, this), collisionInformation);// && (__ABS(collisionInformation->solutionVector.direction.y) > __ABS(collisionInformation->solutionVector.direction.x));

	if(addForce)
	{

		Actor_addForce(__SAFE_CAST(Actor, this), &force);

	}
	return collisionResult;
}
