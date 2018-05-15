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
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other



// class's constructor
void Paddle::constructor(Paddle this, PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "Paddle::constructor: null this");

	// construct base
	Base::constructor((ActorDefinition*)&paddleDefinition->actorDefinition, id, internalId, name);

	// save definition
	this->paddleDefinition = paddleDefinition;
	this->paddleShape = NULL;
}

// class's constructor
void Paddle::destructor(Paddle this)
{
	ASSERT(this, "Paddle::destructor: null this");

	// delete the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void Paddle::ready(Paddle this, bool recursive)
{
	ASSERT(this, "Paddle::ready: null this");

	// call base
	Base::ready(this, recursive);

	Paddle::stopMovement(this);

	this->paddleShape = VirtualList::back(this->shapes);

	NM_ASSERT(__GET_CAST(Box, this->paddleShape), "Paddle::ready: sencod shape must be a box");
}

// start moving
void Paddle::startMovement(Paddle this __attribute__ ((unused)))
{
}

// move back to ejector
void Paddle::stopMovement(Paddle this)
{
	// stop movement
	Actor::stopAllMovement(__SAFE_CAST(Actor, this));
}

// state's handle message
bool Paddle::handleMessage(Paddle this, Telegram telegram)
{
	ASSERT(this, "Paddle::handleMessage: null this");

	switch(Telegram::getMessage(telegram))
	{
	}

	return Base::handleMessage(this, telegram);
}

void Paddle::setExtraInfo(Paddle this __attribute__ ((unused)), void* extraInfo __attribute__ ((unused)))
{
	ASSERT(this, "Paddle::setExtraInfo: null this");
}


void Paddle::transform(Paddle this, const Transformation* environmentTransform, u8 invalidateTransformationFlag)
{
	Base::transform(this, environmentTransform, invalidateTransformationFlag);

//	Shape::show(VirtualList::back(this->shapes));
}

void Paddle::syncRotationWithBody(Paddle this __attribute__ ((unused)))
{

}

void Paddle::moveTowards(Paddle this, Direction direction)
{
	Force force =
	{
		__FIX10_6_MULT(this->paddleDefinition->force.x, __I_TO_FIX10_6(direction.x)),
		__FIX10_6_MULT(this->paddleDefinition->force.y, __I_TO_FIX10_6(direction.y)),
		0
	};

	Actor::addForce(__SAFE_CAST(Actor, this), &force);
}

void Paddle::stopTowards(Paddle this, Direction direction)
{
	u16 axis = __NO_AXIS;
	axis |= direction.x ? __X_AXIS : 0;
	axis |= direction.y ? __Y_AXIS : 0;
	Actor::stopMovement(__SAFE_CAST(Actor, this), axis);
}

void Paddle::retract(Paddle this)
{
	if(Shape::isActive(this->paddleShape))
	{
		AnimatedEntity::playAnimation(__SAFE_CAST(AnimatedEntity, this), "Retracted");

		Shape::setActive(this->paddleShape, false);
	}
}

void Paddle::eject(Paddle this)
{
	AnimatedEntity::playAnimation(__SAFE_CAST(AnimatedEntity, this), "Ejected");

	Shape::setActive(this->paddleShape, true);
}

bool Paddle::mustBounce(Paddle this __attribute__ ((unused)))
{
	ASSERT(this, "Paddle::mustBounce: null this");

	return false;
}