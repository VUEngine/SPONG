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
#include <I18n.h>
#include <Utilities.h>
#include <GameEvents.h>
#include <EventManager.h>
#include "TransitionEntity.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(TransitionEntity, AnimatedEntity);


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

void TransitionEntity_localize(TransitionEntity this);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other
__CLASS_NEW_DEFINITION(TransitionEntity, const TransitionEntityDefinition* TransitionEntityDefinition, s16 id, s16 internalId, const char* const name)
__CLASS_NEW_END(TransitionEntity, TransitionEntityDefinition, id, internalId, name);

// class's constructor
void TransitionEntity_constructor(TransitionEntity this, const TransitionEntityDefinition* TransitionEntityDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "TransitionEntity::constructor: null this");

	// construct base object
	__CONSTRUCT_BASE(AnimatedEntity, (AnimatedEntityDefinition*)TransitionEntityDefinition, id, internalId, name);
}

// class's destructor
void TransitionEntity_destructor(TransitionEntity this)
{
	ASSERT(this, "TransitionEntity::destructor: null this");

	// destroy the super object
	// must always be called at the end of the destructor
	__DESTROY_BASE;
}

// handle event
void TransitionEntity_onTransitionComplete(TransitionEntity this __attribute__ ((unused)), Object eventFirer __attribute__ ((unused)))
{
	Object_fireEvent(__SAFE_CAST(Object, Game_getCurrentState(Game_getInstance())), kEventTransitionOutComplete);
}