/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */

#ifndef TRANSITION_ENTITY_H_
#define TRANSITION_ENTITY_H_


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <AnimatedEntity.h>
#include <Body.h>


//---------------------------------------------------------------------------------------------------------
//											TYPE DEFINITIONS
//---------------------------------------------------------------------------------------------------------

typedef const AnimatedEntitySpec TransitionEntitySpec;
typedef const TransitionEntitySpec TransitionEntityROMSpec;


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DECLARATION
//---------------------------------------------------------------------------------------------------------

class TransitionEntity : AnimatedEntity
{
	void constructor(const TransitionEntitySpec* TransitionEntitySpec, int16 id, int16 internalId, const char* const name);
	void onTransitionInComplete(Object eventFirer);
	void onTransitionOutComplete(Object eventFirer);
}


#endif
