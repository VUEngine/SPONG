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
#include <CollisionManager.h>
#include <Optics.h>
#include <CollisionManager.h>
#include <Utilities.h>
#include <GameEvents.h>
#include <MessageDispatcher.h>
#include <Player.h>
#include <debugUtilities.h>
#include "PongBallLight.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define Z_SCALING_COMPENSATION				__F_TO_FIX10_6(2.5f)
#define WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING		500


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void PongBallLight::constructor(PongBallLightSpec* PongBallLightSpec, int16 internalId, const char* const name)
{
	// construct base
	Base::constructor((EntitySpec*)&PongBallLightSpec->entitySpec, internalId, name);

	// save spec
	this->pongBall = NULL;
	this->pongBallInitialZDistance = __I_TO_FIX10_6(1);
	this->followPongBall = false;
}

void PongBallLight::destructor()
{
	Object::removeEventListener(this->pongBall, Object::safeCast(this), (EventListener)PongBallLight::onPongBallHitFloor, kEventPongBallHitFloor);
	Object::removeEventListener(this->pongBall, Object::safeCast(this), (EventListener)PongBallLight::onPongBallHitCeiling, kEventPongBallHitCeiling);
	this->pongBall = NULL;

	// delete the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void PongBallLight::ready(bool recursive)
{
	// call base
	Base::ready(this, recursive);

	this->pongBall = PongBall::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)PONG_BALL_NAME, true));
	NM_ASSERT(this->pongBall, "PongBallLight::ready: null pongBall");
	this->pongBallInitialZDistance = this->transformation.globalPosition.z -  SpatialObject::getPosition(this->pongBall)->z;
	this->followPongBall = true;
	Object::addEventListener(this->pongBall, Object::safeCast(this), (EventListener)PongBallLight::onPongBallHitFloor, kEventPongBallHitFloor);
	Object::addEventListener(this->pongBall, Object::safeCast(this), (EventListener)PongBallLight::onPongBallHitCeiling, kEventPongBallHitCeiling);
}

void PongBallLight::update(uint32 elapsedTime)
{
	Base::update(this, elapsedTime);

	this->followPongBall |= PongBall::isRolling(Player::getPongBall(Player::getInstance()));

	if(this->followPongBall)
	{
		const Vector3D* pongBallPosition =  SpatialObject::getPosition(this->pongBall);

		Vector3D localPosition = this->transformation.localPosition;

		localPosition.x  = pongBallPosition->x;
		localPosition.y  = pongBallPosition->y;

		 Entity::setLocalPosition(this, &localPosition);

		Scale localScale = this->transformation.localScale;
		localScale.x = localScale.y = __FIX10_6_TO_FIX7_9(__FIX10_6_MULT(__I_TO_FIX10_6(1), __FIX10_6_DIV(this->pongBallInitialZDistance, this->transformation.globalPosition.z - (pongBallPosition->z - Z_SCALING_COMPENSATION))));
		Container::setLocalScale(Container::safeCast(this), &localScale);
	}
}

void PongBallLight::onPongBallHitFloor(Object eventFirer __attribute__ ((unused)))
{
	this->followPongBall = false;
	MessageDispatcher::dispatchMessage(WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING, Object::safeCast(this), Object::safeCast(this), kMessageFollowPongBall, NULL);
}

void PongBallLight::onPongBallHitCeiling(Object eventFirer __attribute__ ((unused)))
{
	this->followPongBall = false;
	MessageDispatcher::dispatchMessage(WAIT_AFTER_PONG_BALL_HIT_FLOOR_OR_CEILING, Object::safeCast(this), Object::safeCast(this), kMessageFollowPongBall, NULL);
/*
	PixelVector displacement = *Sprite::getDisplacement(VirtualList::front(this->sprites));
	displacement.parallax = -5;
	Sprite::setDisplacement(VirtualList::front(this->sprites), &displacement);

	Scale localScale = this->transformation.localScale;
	localScale.x = localScale.y = __FIX10_6_TO_FIX7_9(__FIX10_6_MULT(__I_TO_FIX10_6(1), __I_TO_FIX10_6(2)));
	Container::setLocalScale(Container::safeCast(this), &localScale);
	*/
}

bool PongBallLight::handleMessage(Telegram telegram)
{
	// process message
	switch(Telegram::getMessage(telegram))
	{
		case kMessageFollowPongBall:
			{
				PixelVector displacement = *Sprite::getDisplacement(VirtualList::front(this->sprites));
				displacement.parallax = 0;
				Sprite::setDisplacement(VirtualList::front(this->sprites), &displacement);

				this->followPongBall = true;
			}

			return true;
			break;
	}

	return false;
}
