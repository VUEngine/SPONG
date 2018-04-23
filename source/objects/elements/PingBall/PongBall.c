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
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define START_X_FORCE 	__I_TO_FIX10_6(Utilities_random(seed, 100))
#define START_Y_FORCE 	__I_TO_FIX10_6(Utilities_random(seed, 100))
#define START_Z_FORCE	0

#define MINIMUM_HORIZONTAL_SPEED						__I_TO_FIX10_6(5)
#define MINIMUM_DEPTH_SPEED								__I_TO_FIX10_6(15)
#define FORCE_TO_APPLY_WHEN_VERTICAL_SPEED_IS_ZERO		__I_TO_FIX10_6(-60)
#define FORCE_DECREASE_PER_CYCLE						__I_TO_FIX10_6(1)
#define SPEED_X_MULTIPLIER								__I_TO_FIX10_6(3)
#define SPEED_Y_MULTIPLIER								__I_TO_FIX10_6(2)

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
	this->modifierForce = (Vector3D){0, 0, 0};
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

	Velocity velocity = Body_getVelocity(this->body);
	Rotation localRotation = this->transformation.localRotation;

	if(0 <= velocity.x)
	{
		localRotation.z += __FIX10_6_TO_I(Vector3D_squareLength(Body_getVelocity(this->body))) >> 4;
	}
	else
	{
		localRotation.z -= __FIX10_6_TO_I(Vector3D_squareLength(Body_getVelocity(this->body))) >> 4;
	}

	Entity_setLocalRotation(__SAFE_CAST(Entity, this), &localRotation);

}

// start moving
void PongBall_startMovement(PongBall this)
{
	long seed = Utilities_randomSeed();

	Force force =
	{
		START_X_FORCE,
		START_Y_FORCE,
		START_Z_FORCE,
	};

	Actor_addForce(__SAFE_CAST(Actor, this), &force);
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

	Velocity velocityModifier = (Vector3D){0, 0, 0};

	switch(__VIRTUAL_CALL(SpatialObject, getInGameType, collidingObject))
	{
		case kPaddleType:

			velocityModifier.x = __FIX10_6_MULT(this->transformation.globalPosition.x - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->x, SPEED_X_MULTIPLIER);
			velocityModifier.y = __FIX10_6_MULT(this->transformation.globalPosition.y - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->y, SPEED_Y_MULTIPLIER);
			break;

		case kWall:
		{
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

	if(velocityModifier.x |velocityModifier.y)
	{
		Velocity velocity = Body_getVelocity(this->body);

		if(velocity.z)
		{
			// make sure a minimum vertical speed
			if(0 > velocity.z && __ABS(velocity.z) < MINIMUM_DEPTH_SPEED)
			{
				velocityModifier.z = -(MINIMUM_DEPTH_SPEED - __ABS(velocity.z));
			}
		}
		else
		{
			// don't allow me to move flat
				velocityModifier.z = -(MINIMUM_DEPTH_SPEED - __ABS(velocity.z));
		}

		if(velocity.x)
		{
			// make sure a minimum vertical speed
			if(__ABS(velocity.x) < MINIMUM_HORIZONTAL_SPEED)
			{
				if(0 <= velocity.x)
				{
					velocityModifier.x = (MINIMUM_HORIZONTAL_SPEED);
				}
				else
				{
					velocityModifier.x = -(MINIMUM_HORIZONTAL_SPEED);
				}
			}
		}
	}

	Body_modifyVelocity(this->body, &velocityModifier);

	return collisionResult;
}

fix10_6 PongBall_getFrictionOnCollision(PongBall this __attribute__ ((unused)), SpatialObject collidingObject __attribute__ ((unused)), const Vector3D* collidingObjectNormal __attribute__ ((unused)))
{
	ASSERT(this, "PongBall::getFrictionOnCollision: null this");

	return 0;
}

fix10_6 PongBall_getSurroundingFrictionCoefficient(PongBall this __attribute__ ((unused)))
{
	return 0;
}