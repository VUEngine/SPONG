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

	void constructor(PaddleSpec* paddleSpec, s16 id, s16 internalId, const char* const name);
	void startMovement();
	void moveTowards(Direction direction);
	void stopTowards(Direction direction);
	void retract();
	void eject();
	override void ready(bool recursive);
	override bool handleMessage(Telegram telegram);
	override void setExtraInfo(void* extraInfo);
	override void transform(const Transformation* environmentTransform, u8 invalidateTransformationFlag);
	override void syncRotationWithBody();
	override bool mustBounce();
	override bool enterCollision(const CollisionInformation* collisionInformation);
	override void exitCollision(Shape shape, Shape shapeNotCollidingAnymore, bool isShapeImpenetrable);
	override bool isSubjectToGravity(Acceleration gravity);
}


#endif
