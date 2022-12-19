/**
 * SPONG
 *
 * © Jorge Eremiev <jorgech3@gmail.com> and Christian Radke <c.radke@posteo.de>
 *
 * For the full copyright and license information, please view the LICENSE file
 * that was distributed with this source code.
 */


//---------------------------------------------------------------------------------------------------------
//												INCLUDES
//---------------------------------------------------------------------------------------------------------

#include <VUEngine.h>
#include <I18n.h>
#include <Utilities.h>
#include <GameEvents.h>
#include "TransitionEntity.h"


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void TransitionEntity::constructor(const TransitionEntitySpec* transitionEntitySpec, int16 internalId, const char* const name)
{
	// construct base object
	Base::constructor((AnimatedEntitySpec*)transitionEntitySpec, internalId, name);

	// add event listeners
	ListenerObject::addEventListener(VUEngine::getCurrentState(VUEngine::getInstance()), ListenerObject::safeCast(this), (EventListener)TransitionEntity::onShowScreen, kEventShowScreen);
}

void TransitionEntity::destructor()
{
	// remove event listeners
	ListenerObject::removeEventListener(VUEngine::getCurrentState(VUEngine::getInstance()), ListenerObject::safeCast(this), (EventListener)TransitionEntity::onShowScreen, kEventShowScreen);

	// destroy the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

// handle event
void TransitionEntity::onTransitionInComplete(ListenerObject eventFirer __attribute__ ((unused)))
{
	ListenerObject::fireEvent(VUEngine::getCurrentState(VUEngine::getInstance()), kEventTransitionInComplete);
}

void TransitionEntity::onTransitionOutComplete(ListenerObject eventFirer __attribute__ ((unused)))
{
	ListenerObject::fireEvent(VUEngine::getCurrentState(VUEngine::getInstance()), kEventTransitionOutComplete);
}

// handle event
void TransitionEntity::onShowScreen(ListenerObject eventFirer __attribute__ ((unused)))
{
	AnimatedEntity::playAnimation(this, "FadeIn");
}
