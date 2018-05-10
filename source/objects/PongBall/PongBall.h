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

#ifndef PONG_BALL_H_
#define PONG_BALL_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Actor.h>
#include <Paddle.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

#define PongBall_METHODS(ClassName)																		\
		Actor_METHODS(ClassName)																		\

#define PongBall_SET_VTABLE(ClassName)																	\
		Actor_SET_VTABLE(ClassName)																		\
		__VIRTUAL_SET(ClassName, PongBall, ready);														\
		__VIRTUAL_SET(ClassName, PongBall, update);														\
		__VIRTUAL_SET(ClassName, PongBall, handleMessage);												\
		__VIRTUAL_SET(ClassName, PongBall, enterCollision);												\
		__VIRTUAL_SET(ClassName, PongBall, getFrictionOnCollision);										\
		__VIRTUAL_SET(ClassName, PongBall, getSurroundingFrictionCoefficient);							\
		__VIRTUAL_SET(ClassName, PongBall, syncRotationWithBody);										\

__CLASS(PongBall);

#define PongBall_ATTRIBUTES																				\
		Actor_ATTRIBUTES																				\
		/* definition pointer */																		\
		PongBallDefinition* pongBallDefinition;															\
		Force modifierForce;																			\
		int paddleEnum;																					\
		bool rolling;																					\


typedef struct PongBallDefinition
{
	// the base animated entity
	ActorDefinition actorDefinition;

	// minimum velocity when moving
	Velocity minimumVelocity;

	// maximum velocity when moving
	Velocity maximumVelocity;

	// maximum velocity when moving
	Velocity bonusVelocity;

} PongBallDefinition;

typedef const PongBallDefinition PongBallROMDef;


//---------------------------------------------------------------------------------------------------------
//										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

// allocator
__CLASS_NEW_DECLARE(PongBall, PongBallDefinition* pongBallDefinition, s16 id, s16 internalId, const char* const name);

void PongBall_constructor(PongBall this, PongBallDefinition* pongBallDefinition, s16 id, s16 internalId, const char* const name);
void PongBall_destructor(PongBall this);
void PongBall_startMovement(PongBall this);
void PongBall_stopMovement(PongBall this);
void PongBall_ready(PongBall this, bool recursive);
void PongBall_update(PongBall this, u32 elapsedTime);
bool PongBall_handleMessage(PongBall this, Telegram telegram);
bool PongBall_enterCollision(PongBall this, const CollisionInformation* collisionInformation);
fix10_6 PongBall_getFrictionOnCollision(PongBall this, SpatialObject collidingObject, const Vector3D* collidingObjectNormal);
fix10_6 PongBall_getSurroundingFrictionCoefficient(PongBall this);
int PongBall_getPaddleEnum(PongBall this);
void PongBall_startRolling(PongBall this);
void PongBall_syncRotationWithBody(PongBall this);


#endif
