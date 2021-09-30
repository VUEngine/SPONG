/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef PADDLE_H_
#define PADDLE_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Actor.h>
#include <Shape.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------


typedef struct PaddleSpec
{
	// the base animated entity
	ActorSpec actorSpec;

	// force to apply
	Force force;

} PaddleSpec;

typedef const PaddleSpec PaddleROMSpec;


enum PlayerPaddles
{
	kLeftPaddle = 0,
	kRightPaddle,
	kNoPaddle
};


class Paddle : Actor
{
	/* spec pointer */
	PaddleSpec* paddleSpec;
	Shape paddleShape;
	bool mustBounce;

	void constructor(PaddleSpec* paddleSpec, int16 id, int16 internalId, const char* const name);
	void startMovement();
	void moveTowards(Direction direction);
	void stopTowards(Direction direction);
	void retract();
	void eject();
	override void ready(bool recursive);
	override bool handleMessage(Telegram telegram);
	override void setExtraInfo(void* extraInfo);
	override void transform(const Transformation* environmentTransform, uint8 invalidateTransformationFlag);
	override void syncRotationWithBody();
	override bool mustBounce();
	override bool enterCollision(const CollisionInformation* collisionInformation);
	override void exitCollision(Shape shape, Shape shapeNotCollidingAnymore, bool isShapeImpenetrable);
	override bool isSubjectToGravity(Acceleration gravity);
}


#endif
