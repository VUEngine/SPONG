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

#ifndef PONG_BALL_SHADOW_H_
#define PONG_BALL_SHADOW_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <Entity.h>
#include <PongBall.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

#define PongBallShadow_METHODS(ClassName)																\
		Entity_METHODS(ClassName)																		\

#define PongBallShadow_SET_VTABLE(ClassName)															\
		Entity_SET_VTABLE(ClassName)																	\
		__VIRTUAL_SET(ClassName, PongBallShadow, ready);												\
		__VIRTUAL_SET(ClassName, PongBallShadow, update);												\
		__VIRTUAL_SET(ClassName, PongBallShadow, handleMessage);										\

__CLASS(PongBallShadow);

#define PongBallShadow_ATTRIBUTES																		\
		Entity_ATTRIBUTES																				\
		/* definition pointer */																		\
		PongBallShadowDefinition* PongBallShadowDefinition;												\
		PongBall pongBall;																				\
		fix10_6 pongBallInitialZDistance;																\
		bool followPongBall;																			\


typedef struct PongBallShadowDefinition
{
	// the base animated entity
	EntityDefinition entityDefinition;

} PongBallShadowDefinition;

typedef const PongBallShadowDefinition PongBallShadowROMDef;


//---------------------------------------------------------------------------------------------------------
//										PUBLIC INTERFACE
//---------------------------------------------------------------------------------------------------------

// allocator
__CLASS_NEW_DECLARE(PongBallShadow, PongBallShadowDefinition* pongBallShadowDefinition, s16 id, s16 internalId, const char* const name);

void PongBallShadow_constructor(PongBallShadow this, PongBallShadowDefinition* pongBallShadowDefinition, s16 id, s16 internalId, const char* const name);
void PongBallShadow_destructor(PongBallShadow this);
void PongBallShadow_ready(PongBallShadow this, bool recursive);
void PongBallShadow_update(PongBallShadow this, u32 elapsedTime);
bool PongBallShadow_handleMessage(PongBallShadow this, Telegram telegram);

#endif