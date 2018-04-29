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

#include <LocalizedEntity.h>
#include <Game.h>
#include <I18n.h>
#include <Utilities.h>
#include "LocalizedEntity.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(LocalizedEntity, AnimatedEntity);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// always call these two macros next to each other
__CLASS_NEW_DEFINITION(LocalizedEntity, const LocalizedEntityDefinition* localizedEntityDefinition, s16 id, s16 internalId, const char* const name)
__CLASS_NEW_END(LocalizedEntity, localizedEntityDefinition, id, internalId, name);

// class's constructor
void LocalizedEntity_constructor(LocalizedEntity this, const LocalizedEntityDefinition* localizedEntityDefinition, s16 id, s16 internalId, const char* const name)
{
	ASSERT(this, "LocalizedEntity::constructor: null this");

	// construct base object
	__CONSTRUCT_BASE(AnimatedEntity, (AnimatedEntityDefinition*)localizedEntityDefinition, id, internalId, name);
}

// class's destructor
void LocalizedEntity_destructor(LocalizedEntity this)
{
	ASSERT(this, "LocalizedEntity::destructor: null this");

	// destroy the super object
	// must always be called at the end of the destructor
	__DESTROY_BASE;
}

void LocalizedEntity_ready(LocalizedEntity this, bool recursive)
{
	ASSERT(this, "LocalizedEntity::ready: null this");

	// call base
	__CALL_BASE_METHOD(AnimatedEntity, ready, this, recursive);

	// translate entity
	char* language = Utilities_itoa(I18n_getActiveLanguage(I18n_getInstance()), 10, 1);
	AnimatedEntity_playAnimation(__SAFE_CAST(AnimatedEntity, this), language);

}
