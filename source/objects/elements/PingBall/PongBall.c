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
#include "PongBall.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define START_X_FORCE 	__I_TO_FIX10_6(Utilities_random(seed, 150))
#define START_Y_FORCE 	__I_TO_FIX10_6(Utilities_random(seed, 150))
#define START_Z_FORCE	0

#define MINIMUM_HORIZONTAL_SPEED						__F_TO_FIX10_6(4.5f)
#define MINIMUM_VERTICAL_SPEED							__F_TO_FIX10_6(1.0f)
#define MINIMUM_DEPTH_SPEED								__I_TO_FIX10_6(8)
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
	this->paddleEnum = kNoPaddle;
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
	const char* paddleName = PADDLE_LEFT_NAME;

	if(50 < Utilities_random(Utilities_randomSeed(), 100))
	{
		paddleName = PADDLE_RIGHT_NAME;
	}

	Entity paddle = __SAFE_CAST(Entity, Container_getChildByName(__SAFE_CAST(Container, Game_getStage(Game_getInstance())), (char*)paddleName, true));
	NM_ASSERT(paddle, "PongBall::startMovement: paddle not found");

	Vector3D localPosition = this->transformation.localPosition;
	const Vector3D* paddlePosition = __VIRTUAL_CALL(Container, getPosition, paddle);
	localPosition.x = paddlePosition->x;
	localPosition.y = paddlePosition->y + 0*__F_TO_FIX10_6(Utilities_random(Utilities_randomSeed(), 10) / 100.0f);
	Entity_setLocalPosition(__SAFE_CAST(Entity, this), &localPosition);
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
			{
				if(this->transformation.globalPosition.x < (__SCREEN_WIDTH_METERS >> 1))
				{
					this->paddleEnum = kLeftPaddle;
				}
				else if(this->transformation.globalPosition.x > (__SCREEN_WIDTH_METERS >> 1))
				{
					this->paddleEnum = kRightPaddle;
				}

				velocityModifier.x = __FIX10_6_MULT(this->transformation.globalPosition.x - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->x, SPEED_X_MULTIPLIER);
				velocityModifier.y = __FIX10_6_MULT(this->transformation.globalPosition.y - __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject)->y, SPEED_Y_MULTIPLIER);

				if(!velocityModifier.y)
				{
					velocityModifier.y = __FIX10_6_DIV(__F_TO_FIX10_6(Utilities_random(Utilities_randomSeed(), 10) - 5), __I_TO_FIX10_6(100));
				}

				Object_fireEvent(__SAFE_CAST(Object, this), kEventPongBallHitPaddle);
			}

			break;

		case kCeiling:
			{
				PRINT_TIME(10,10);
				Object_fireEvent(__SAFE_CAST(Object, this), kEventPongBallHitCeiling);
				break;
			}

		case kFloor:
			{
				const Vector3D* collidingObjectPosition = __VIRTUAL_CALL(SpatialObject, getPosition, collidingObject);

				if(this->transformation.globalPosition.x < collidingObjectPosition->x - __PIXELS_TO_METERS(16))
				{
					Object_fireEvent(__SAFE_CAST(Object, this), kEventPongBallHitFloor);
				}
				else if(this->transformation.globalPosition.x > collidingObjectPosition->x + __PIXELS_TO_METERS(16))
				{
					Object_fireEvent(__SAFE_CAST(Object, this), kEventPongBallHitFloor);
				}
			}

			break;

		case kWall:
/*
			velocityModifier.y = __FIX10_6_MULT(__I_TO_FIX10_6(1) - __ABS(collisionInformation->solutionVector.direction.y), SPEED_Y_MULTIPLIER);

			if(0 > collisionInformation->solutionVector.direction.y)
			{
				velocityModifier.y = __FIX10_6_MULT(__I_TO_FIX10_6(-1), velocityModifier.y);
			}
			*/
			break;
	}

	bool collisionResult = Actor_enterCollision(__SAFE_CAST(Actor, this), collisionInformation);// && (__ABS(collisionInformation->solutionVector.direction.y) > __ABS(collisionInformation->solutionVector.direction.x));

	Velocity velocity = Body_getVelocity(this->body);

	if(velocityModifier.x | velocityModifier.y)
	{
		if(velocity.z)
		{
			// make sure a minimum vertical speed
			if(0 > velocity.z && __ABS(velocity.z) < MINIMUM_DEPTH_SPEED)
			{
				velocityModifier.z = -MINIMUM_DEPTH_SPEED;
			}
		}
		else
		{
			// don't allow me to move flat
				velocityModifier.z = -MINIMUM_DEPTH_SPEED;
		}
	}

	if(velocityModifier.x)
	{		// make sure a minimum vertical speed
		if(__ABS(velocity.x) < MINIMUM_HORIZONTAL_SPEED)
		{
			if(0 <= velocity.x)
			{
				velocityModifier.x = MINIMUM_HORIZONTAL_SPEED;
			}
			else
			{
				velocityModifier.x = -MINIMUM_HORIZONTAL_SPEED;
			}
		}
	}


	if(velocityModifier.y)
	{
		if(__ABS(velocity.y) < MINIMUM_VERTICAL_SPEED)
		{
			if(0 < velocity.y)
			{
				velocityModifier.y = MINIMUM_VERTICAL_SPEED;
			}
			else if(0 > velocity.y)
			{
				velocityModifier.y = -MINIMUM_VERTICAL_SPEED;
			}
			else if(this->transformation.globalPosition.y > (__SCREEN_HEIGHT_METERS >> 1))
			{

				velocityModifier.y = -MINIMUM_VERTICAL_SPEED;
			}
			else
			{
				velocityModifier.y = MINIMUM_VERTICAL_SPEED;
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

int PongBall_getPaddleEnum(PongBall this)
{
	return this->paddleEnum;
}