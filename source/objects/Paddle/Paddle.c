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
#include <Box.h>
#include <Optics.h>
#include <MessageDispatcher.h>
#include "Paddle.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define HORIZONTAL_FORCE		80
#define VERTICAL_FORCE			80


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void Paddle::constructor(PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name)
{
	// construct base
	Base::constructor((ActorDefinition*)&paddleDefinition->actorDefinition, id, internalId, name);

	// save definition
	this->paddleDefinition = paddleDefinition;
	this->paddleShape = NULL;
}

void Paddle::destructor()
{
	// delete the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void Paddle::ready(bool recursive)
{
	// call base
	Base::ready(this, recursive);

	Paddle::stopMovement(this);

	this->paddleShape = VirtualList::back(this->shapes);

	NM_ASSERT(__GET_CAST(Box, this->paddleShape), "Paddle::ready: sencod shape must be a box");
}

// start moving
void Paddle::startMovement()
{
}

// move back to ejector
void Paddle::stopMovement()
{
	// stop movement
	Actor::stopAllMovement(Actor::safeCast(this));
}

// state's handle message
bool Paddle::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
	}

	return Base::handleMessage(this, telegram);
}

void Paddle::setExtraInfo(void* extraInfo __attribute__ ((unused)))
{}


void Paddle::transform(const Transformation* environmentTransform, u8 invalidateTransformationFlag)
{
	Base::transform(this, environmentTransform, invalidateTransformationFlag);

//	Shape::show(VirtualList::back(this->shapes));
}

void Paddle::syncRotationWithBody()
{

}

void Paddle::moveTowards(Direction direction)
{
	Force force =
	{
		__FIX10_6_MULT(this->paddleDefinition->force.x, __I_TO_FIX10_6(direction.x)),
		__FIX10_6_MULT(this->paddleDefinition->force.y, __I_TO_FIX10_6(direction.y)),
		0
	};

	Actor::addForce(Actor::safeCast(this), &force);
}

void Paddle::stopTowards(Direction direction)
{
	u16 axis = __NO_AXIS;
	axis |= direction.x ? __X_AXIS : 0;
	axis |= direction.y ? __Y_AXIS : 0;
	Actor::stopMovement(Actor::safeCast(this), axis);
}

void Paddle::retract()
{
	if(Shape::isActive(this->paddleShape))
	{
		AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this), "Retracted");

		Shape::setActive(this->paddleShape, false);
	}
}

void Paddle::eject()
{
	AnimatedEntity::playAnimation(AnimatedEntity::safeCast(this), "Ejected");

	Shape::setActive(this->paddleShape, true);
}

bool Paddle::mustBounce()
{
	return false;
}