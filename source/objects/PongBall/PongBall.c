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
#include <CollisionManager.h>
#include <Optics.h>
#include <CollisionManager.h>
#include <Utilities.h>
#include <GameEvents.h>
#include <MessageDispatcher.h>
#include <Player.h>
#include <debugUtilities.h>
#include "PongBall.h"


//---------------------------------------------------------------------------------------------------------
//											CLASS'S MACROS
//---------------------------------------------------------------------------------------------------------

#define START_X_FORCE 	__I_TO_FIX10_6(Utilities::random(seed, 150))
#define START_Y_FORCE 	__I_TO_FIX10_6(Utilities::random(seed, 150))
#define START_Z_FORCE	0

#define MINIMUM_HORIZONTAL_SPEED						__F_TO_FIX10_6(7.5f)
#define MINIMUM_VERTICAL_SPEED							__F_TO_FIX10_6(4.0f)
#define MINIMUM_DEPTH_SPEED								__I_TO_FIX10_6(9)
#define FORCE_TO_APPLY_WHEN_VERTICAL_SPEED_IS_ZERO		__I_TO_FIX10_6(-60)
#define FORCE_DECREASE_PER_CYCLE						__I_TO_FIX10_6(1)
#define SPEED_X_MULTIPLIER								__I_TO_FIX10_6(2)
#define SPEED_Y_MULTIPLIER								__I_TO_FIX10_6(2)

//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void PongBall::constructor(PongBallDefinition* pongBallDefinition, s16 id, s16 internalId, const char* const name)
{
	// construct base
	Base::constructor((ActorDefinition*)&pongBallDefinition->actorDefinition, id, internalId, name);

	// save definition
	this->pongBallDefinition = pongBallDefinition;
	this->modifierForce = (Vector3D){0, 0, 0};
	this->paddleEnum = kNoPaddle;
	this->rolling = false;
	this->particles = NULL;

	this->transformation.localScale = (Scale){__F_TO_FIX7_9(1.75f), __F_TO_FIX7_9(1.75f), __F_TO_FIX7_9(1.75f)};
}

// class's constructor
void PongBall::destructor()
{
	// delete the super object
	// must always be called at the end of the destructor
	Base::destructor();
}

void PongBall::ready(bool recursive)
{
	// call base
	Base::ready(this, recursive);

	this->particles = ParticleSystem::safeCast(Container::getChildByName(Container::safeCast(this), "Partcls", true));

	ParticleSystem::setLoop(this->particles, false);

	PongBall::startMovement(this);
}

void PongBall::transform(const Transformation* environmentTransform, u8 invalidateTransformationFlag)
{
	Base::transform(this, environmentTransform, invalidateTransformationFlag);

	Velocity velocity = Body::getVelocity(this->body);
	Rotation localRotation = this->transformation.localRotation;

	if(0 <= velocity.x)
	{
		localRotation.z += __FIX10_6_TO_I(Vector3D::squareLength(Body::getVelocity(this->body))) >> 4;
	}
	else
	{
		localRotation.z -= __FIX10_6_TO_I(Vector3D::squareLength(Body::getVelocity(this->body))) >> 4;
	}

	Entity::setLocalRotation(this, &localRotation);
}

void PongBall::update(u32 elapsedTime)
{
	Base::update(this, elapsedTime);
/*
	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		CommunicationManager::sendAndReceiveDataAsync(CommunicationManager::getInstance(), (BYTE*)&this->transformation.localPosition, sizeof(Vector3D), (EventListener)PongBall::onPositionTransmitted, Object::safeCast(this));
	}
	*/
}

void PongBall::onPositionTransmitted(Object eventFirer __attribute__((unused)))
{
	if(kPlayerTwo == Player::getPlayerNumber(Player::getInstance()))
	{
		const Vector3D* otherWorldPosition = (Vector3D*)CommunicationManager::getData(CommunicationManager::getInstance());
		Entity::setLocalPosition(this, otherWorldPosition);
	}
}

// start moving
void PongBall::startMovement()
{
	const char* paddleName = PADDLE_LEFT_NAME;

	if(50 < Utilities::random(Utilities::randomSeed(), 100))
	{
		paddleName = PADDLE_RIGHT_NAME;
	}

	Entity paddle = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), (char*)paddleName, true));
	NM_ASSERT(paddle, "PongBall::startMovement: paddle not found");

	Vector3D localPosition = this->transformation.localPosition;
	const Vector3D* paddlePosition =  Entity::getPosition(paddle);
	localPosition.x = paddlePosition->x;
	localPosition.y = paddlePosition->y + 0*__F_TO_FIX10_6(Utilities::random(Utilities::randomSeed(), 10) / 100.0f);
	Entity::setLocalPosition(this, &localPosition);
}

// move back to ejector
void PongBall::stopMovement()
{
	// stop movement
	Actor::stopAllMovement(Actor::safeCast(this));
}

// state's handle message
bool PongBall::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
	}

	return Base::handleMessage(this, telegram);
}

bool PongBall::enterCollision(const CollisionInformation* collisionInformation)
{
	ASSERT(collisionInformation->collidingShape, "Hero::enterCollision: null collidingObjects");

	Shape collidingShape = collisionInformation->collidingShape;
	SpatialObject collidingObject = Shape::getOwner(collidingShape);

	Velocity velocityModifier = (Vector3D){0, 0, 0};

	bool hitFloor = false;

	switch( SpatialObject::getInGameType(collidingObject))
	{
		case kPaddleType:

//			PRINT_TEXT("Paddle", 20, 3);
			{
				if(this->transformation.globalPosition.x < (__SCREEN_WIDTH_METERS >> 1))
				{
					this->paddleEnum = kLeftPaddle;
				}
				else if(this->transformation.globalPosition.x > (__SCREEN_WIDTH_METERS >> 1))
				{
					this->paddleEnum = kRightPaddle;
				}

				velocityModifier.x = __FIX10_6_MULT(this->transformation.globalPosition.x -  SpatialObject::getPosition(collidingObject)->x, SPEED_X_MULTIPLIER);
				velocityModifier.y = __FIX10_6_MULT(this->transformation.globalPosition.y -  SpatialObject::getPosition(collidingObject)->y, SPEED_Y_MULTIPLIER);

				if(!velocityModifier.y)
				{
					velocityModifier.y = __FIX10_6_DIV(__F_TO_FIX10_6(Utilities::random(Utilities::randomSeed(), 10) - 5), __I_TO_FIX10_6(100));
				}

				if(this->particles)
				{
					ParticleSystem::setLoop(this->particles, false);
					ParticleSystem::start(this->particles);
				}

				this->rolling = false;

				Body::setMaximumVelocity(this->body, this->pongBallDefinition->maximumVelocity);

				Object::fireEvent(this, kEventPongBallHitPaddle);
			}

			break;

		case kCeiling:
			{
//			PRINT_TEXT("Ceil  ", 20, 3);
//				Object::fireEvent(this, kEventPongBallHitCeiling);
				break;
			}

		case kFloor:
			{
//			PRINT_TEXT("Floor  ", 20, 3);
				const Vector3D* collidingObjectPosition =  SpatialObject::getPosition(collidingObject);

				Object::fireEvent(this, kEventPongBallHitFloor);
/*
				if(this->transformation.globalPosition.x < collidingObjectPosition->x - __PIXELS_TO_METERS(8))
				{
					Object::fireEvent(this, kEventPongBallHitFloor);
				}
				else if(this->transformation.globalPosition.x > collidingObjectPosition->x + __PIXELS_TO_METERS(8))
				{
					Object::fireEvent(this, kEventPongBallHitFloor);
				}
				*/
			}

			break;

		case kWall:

//			PRINT_TEXT("Wall  ", 20, 3);
			if(this->rolling)
			{
				velocityModifier.x = __FIX10_6_MULT(__I_TO_FIX10_6(1) - __ABS(collisionInformation->solutionVector.direction.x), SPEED_Y_MULTIPLIER);

				if(0 > collisionInformation->solutionVector.direction.x)
				{
					velocityModifier.x = -velocityModifier.x;
				}

				velocityModifier.y = __FIX10_6_MULT(__I_TO_FIX10_6(1) - __ABS(collisionInformation->solutionVector.direction.y), SPEED_Y_MULTIPLIER);

				if(0 > collisionInformation->solutionVector.direction.y)
				{
					velocityModifier.y = -velocityModifier.y;
				}
			}
			else if(!collisionInformation->solutionVector.direction.y)
			{
				if(this->transformation.globalPosition.y < __SCREEN_HEIGHT_METERS / 2)
				{
					velocityModifier.y = MINIMUM_VERTICAL_SPEED;
				}
				else
				{
					velocityModifier.y = -MINIMUM_VERTICAL_SPEED;
				}
			}
			break;
	}

	bool collisionResult = Base::enterCollision(this, collisionInformation);// && (__ABS(collisionInformation->solutionVector.direction.y) > __ABS(collisionInformation->solutionVector.direction.x));

	Velocity velocity = Body::getVelocity(this->body);

	if(!this->rolling && (velocityModifier.x | velocityModifier.y))
	{
		if(velocity.z)
		{
			// make sure a minimum vertical speed
			if(0 > velocity.z && __ABS(velocity.z) < MINIMUM_DEPTH_SPEED)
			{
				velocityModifier.z = -MINIMUM_DEPTH_SPEED;
			}
		}
		else
		{
			// don't allow me to move flat
				velocityModifier.z = -MINIMUM_DEPTH_SPEED;
		}
	}
	else if(this->rolling)
	{
		velocityModifier.z = -Body::getVelocity(this->body).z;
	}

	if(velocityModifier.x)
	{		// make sure a minimum vertical speed
		if(__ABS(velocity.x) < MINIMUM_HORIZONTAL_SPEED)
		{
			if(0 <= velocity.x)
			{
				velocityModifier.x = MINIMUM_HORIZONTAL_SPEED;
			}
			else
			{
				velocityModifier.x = -MINIMUM_HORIZONTAL_SPEED;
			}
		}
	}

	if(velocityModifier.y)
	{
		if(__ABS(velocity.y) < MINIMUM_VERTICAL_SPEED)
		{
			if(0 < velocity.y)
			{
				velocityModifier.y = MINIMUM_VERTICAL_SPEED;
			}
			else if(0 > velocity.y)
			{
				velocityModifier.y = -MINIMUM_VERTICAL_SPEED;
			}
			else if(this->transformation.globalPosition.y > (__SCREEN_HEIGHT_METERS >> 1))
			{

				velocityModifier.y = -MINIMUM_VERTICAL_SPEED;
			}
			else
			{
				velocityModifier.y = MINIMUM_VERTICAL_SPEED;
			}
		}
	}

	Body::modifyVelocity(this->body, &velocityModifier);

	return collisionResult;
}

fix10_6 PongBall::getFrictionOnCollision(SpatialObject collidingObject __attribute__ ((unused)), const Vector3D* collidingObjectNormal __attribute__ ((unused)))
{
	return 0;
}

fix10_6 PongBall::getSurroundingFrictionCoefficient()
{
	return 0;
}

int PongBall::getPaddleEnum()
{
	return this->paddleEnum;
}

void PongBall::startRolling()
{
	this->rolling = true;

	Body::setMaximumVelocity(this->body, this->pongBallDefinition->bonusVelocity);

	Velocity velocity = Body::getVelocity(this->body);

	velocity.x += 0 < velocity.x ? __ABS(this->pongBallDefinition->bonusVelocity.x) : 0 > velocity.x ? -__ABS(this->pongBallDefinition->bonusVelocity.x) : this->transformation.globalPosition.x > (__SCREEN_WIDTH_METERS >> 1) ? -MINIMUM_HORIZONTAL_SPEED : MINIMUM_HORIZONTAL_SPEED;
	velocity.y += 0 < velocity.y ? __ABS(this->pongBallDefinition->bonusVelocity.y) : 0 > velocity.y ? -__ABS(this->pongBallDefinition->bonusVelocity.y) : this->transformation.globalPosition.y > (__SCREEN_HEIGHT_METERS >> 1) ? -MINIMUM_HORIZONTAL_SPEED : MINIMUM_HORIZONTAL_SPEED;
	velocity.z = -velocity.z;
	Body::modifyVelocity(this->body, &velocity);

	if(this->particles)
	{
		ParticleSystem::setLoop(this->particles, true);

		ParticleSystem::start(this->particles);
	}
}

bool PongBall::isRolling()
{
	return this->rolling;
}