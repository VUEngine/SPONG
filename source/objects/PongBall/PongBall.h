/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
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

	void constructor(PongBallSpec* pongBallSpec, int16 id, int16 internalId, const char* const name);
	void startMovement();
	void stopMovement();
	int getPaddleEnum();
	void startRolling();
	bool isRolling();
	override void ready(bool recursive);
	override void update(uint32 elapsedTime);
	override void transform(const Transformation* environmentTransform, uint8 invalidateTransformationFlag);
	override bool handleMessage(Telegram telegram);
	override bool enterCollision(const CollisionInformation* collisionInformation);
	override fix10_6 getFrictionOnCollision(SpatialObject collidingObject, const Vector3D* collidingObjectNormal);
	override fix10_6 getSurroundingFrictionCoefficient();
	override void syncRotationWithBody();
}


#endif
