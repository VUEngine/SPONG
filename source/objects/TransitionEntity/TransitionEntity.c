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

#include <Game.h>
#include <I18n.h>
#include <Utilities.h>
#include <GameEvents.h>
#include "TransitionEntity.h"


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void TransitionEntity::constructor(const TransitionEntitySpec* TransitionEntitySpec, int16 id, int16 internalId, const char* const name)
{
	// construct base object
	Base::constructor((AnimatedEntitySpec*)TransitionEntitySpec, id, internalId, name);

	// add event listeners
	Object::addEventListener(Game::getCurrentState(Game::getInstance()), Object::safeCast(this), (EventListener)TransitionEntity_onShowScreen, kEventShowScreen);
}

void TransitionEntity::destructor()
{
	// remove event listeners
	Object::removeEventListener(Game::getCurrentState(Game::getInstance()), Object::safeCast(this), (EventListener)TransitionEntity_onShowScreen, kEventShowScreen);

	// destroy the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

// handle event
void TransitionEntity::onTransitionInComplete(Object eventFirer __attribute__ ((unused)))
{
	Object::fireEvent(Game::getCurrentState(Game::getInstance()), kEventTransitionInComplete);
}

void TransitionEntity::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	Object::fireEvent(Game::getCurrentState(Game::getInstance()), kEventTransitionOutComplete);
}

// handle event
void TransitionEntity::onShowScreen(Object eventFirer __attribute__ ((unused)))
{
	AnimatedEntity::playAnimation(this, "FadeIn");
}
