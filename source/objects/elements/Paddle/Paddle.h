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

#define Paddle_METHODS(ClassName)																		\
		Actor_METHODS(ClassName)																		\

#define Paddle_SET_VTABLE(ClassName)																	\
		Actor_SET_VTABLE(ClassName)																		\
		__VIRTUAL_SET(ClassName, Paddle, ready);														\
		__VIRTUAL_SET(ClassName, Paddle, handleMessage);												\
		__VIRTUAL_SET(ClassName, Paddle, setExtraInfo);													\
		__VIRTUAL_SET(ClassName, Paddle, transform);													\
		__VIRTUAL_SET(ClassName, Paddle, syncRotationWithBody);											\
		__VIRTUAL_SET(ClassName, Paddle, mustBounce);													\

__CLASS(Paddle);

#define Paddle_ATTRIBUTES																				\
		Actor_ATTRIBUTES																				\
		/* definition pointer */																		\
		PaddleDefinition* paddleDefinition;																\
		Shape paddleShape;																				\


typedef struct PaddleDefinition
{
	// the base animated entity
	ActorDefinition actorDefinition;

	// minimum velocity when moving
	Velocity minimumVelocity;

	// maximum velocity when moving
	Velocity maximumVelocity;

} PaddleDefinition;

typedef const PaddleDefinition PaddleROMDef;


enum PlayerPaddles
{
	kLeftPaddle = 0,
	kRightPaddle,
	kNoPaddle
};


//---------------------------------------------------------------------------------------------------------
//										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

// allocator
__CLASS_NEW_DECLARE(Paddle, PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name);

void Paddle_constructor(Paddle this, PaddleDefinition* paddleDefinition, s16 id, s16 internalId, const char* const name);
void Paddle_destructor(Paddle this);
void Paddle_startMovement(Paddle this);
void Paddle_stopMovement(Paddle this);
void Paddle_ready(Paddle this, bool recursive);
bool Paddle_handleMessage(Paddle this, Telegram telegram);
void Paddle_setExtraInfo(Paddle this, void* extraInfo);
void Paddle_transform(Paddle this, const Transformation* environmentTransform, u8 invalidateTransformationFlag);
void Paddle_syncRotationWithBody(Paddle this);
void Paddle_moveTowards(Paddle this, Direction direction);
void Paddle_stopTowards(Paddle this, Direction direction);
void Paddle_retract(Paddle this);
void Paddle_eject(Paddle this);
bool Paddle_mustBounce(Paddle this);

#endif
