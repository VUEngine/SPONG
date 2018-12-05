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
#include <ParticleSystem.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------


typedef struct PongBallSpec
{
	// the base animated entity
	ActorSpec actorSpec;

	// minimum velocity when moving
	Velocity minimumVelocity;

	// maximum velocity when moving
	Velocity maximumVelocity;

	// maximum velocity when moving
	Velocity bonusVelocity;

} PongBallSpec;

typedef const PongBallSpec PongBallROMSpec;


class PongBall : Actor
{
	/* spec pointer */
	ParticleSystem particles;
	PongBallSpec* pongBallSpec;
	Force modifierForce;
	int paddleEnum;
	bool rolling;

	void constructor(PongBallSpec* pongBallSpec, s16 id, s16 internalId, const char* const name);
	void startMovement();
	void stopMovement();
	int getPaddleEnum();
	void startRolling();
	bool isRolling();
	override void ready(bool recursive);
	override void update(u32 elapsedTime);
	override void transform(const Transformation* environmentTransform, u8 invalidateTransformationFlag);
	override bool handleMessage(Telegram telegram);
	override bool enterCollision(const CollisionInformation* collisionInformation);
	override fix10_6 getFrictionOnCollision(SpatialObject collidingObject, const Vector3D* collidingObjectNormal);
	override fix10_6 getSurroundingFrictionCoefficient();
}


#endif
