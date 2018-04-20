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
#include <Optics.h>
#include <MessageDispatcher.h>
#include "Paddle.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(Paddle, Actor);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other
__CLASS_NEW_DEFINITION(Paddle, PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name)
__CLASS_NEW_END(Paddle, paddleDefinition, id, internalId, name);

// class's constructor
void Paddle_constructor(Paddle this, PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "Paddle::constructor: null this");

	// construct base
	__CONSTRUCT_BASE(Actor, (ActorDefinition*)&paddleDefinition->actorDefinition, id, internalId, name);

	// save definition
	this->paddleDefinition = paddleDefinition;
	this->rotation = (Rotation){0, 0, 0};
}

// class's constructor
void Paddle_destructor(Paddle this)
{
	ASSERT(this, "Paddle::destructor: null this");

	// delete the super object
	// must always be called at the end of the destructor
	__DESTROY_BASE;
}

void Paddle_ready(Paddle this, bool recursive)
{
	ASSERT(this, "Paddle::ready: null this");

	// call base
	__CALL_BASE_METHOD(Actor, ready, this, recursive);

	Paddle_stopMovement(this);

	Entity_setLocalRotation(this, &this->rotation);
}

// start moving
void Paddle_startMovement(Paddle this)
{
}

// move back to ejector
void Paddle_stopMovement(Paddle this)
{
	// stop movement
	Actor_stopAllMovement(__SAFE_CAST(Actor, this));
}

// state's handle message
bool Paddle_handleMessage(Paddle this, Telegram telegram)
{
	ASSERT(this, "Paddle::handleMessage: null this");

	switch(Telegram_getMessage(telegram))
	{
	}

	return false;
}

void Paddle_setExtraInfo(Paddle this, void* extraInfo)
{
	ASSERT(this, "Paddle::setExtraInfo: null this");

	this->rotation = *((Rotation*)extraInfo);
}
