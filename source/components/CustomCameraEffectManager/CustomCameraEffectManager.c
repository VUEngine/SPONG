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

#include <CustomCameraEffectManager.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <Actor.h>
#include <Game.h>
#include <PhysicalWorld.h>
#include <Utilities.h>
#include <VIPManager.h>

#include <debugConfig.h>


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

friend class Camera;


//---------------------------------------------------------------------------------------------------------
//												GLOBALS
//---------------------------------------------------------------------------------------------------------

static Camera _camera = NULL;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// it's a singleton


// class's constructor
void CustomCameraEffectManager::constructor()
{
	// construct base object
	Base::constructor();

	this->lastShakeOffset.x = 0;
	this->lastShakeOffset.y = 0;
	this->lastShakeOffset.z = 0;

	this->shakeTimeLeft = 0;

	_camera = Camera::getInstance();

	NM_ASSERT(_camera, "CustomCameraEffectManager::constructor: null _camera");
}

// class's destructor
void CustomCameraEffectManager::destructor()
{
	// destroy base
	Base::destructor();
}

void CustomCameraEffectManager::startEffect(int effect, va_list args)
{
	switch(effect)
	{
		case kShake:

			CustomCameraEffectManager::fxShakeStart(this, va_arg(args, int));
			break;

		default:

			CameraEffectManager::startEffect(CameraEffectManager::getInstance(), effect, args);
			break;
	}
}

void CustomCameraEffectManager::stopEffect(int effect)
{
	switch(effect)
	{
		case kShake:

			CustomCameraEffectManager::fxShakeStop(this);
			break;

		default:

			Base::stopEffect(this, effect);
			break;
	}
}

bool CustomCameraEffectManager::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kShake:

			CustomCameraEffectManager::onScreenShake(this);
			break;
	}

	return false;
}

// start shaking the screen
void CustomCameraEffectManager::fxShakeStart(u16 duration)
{
	// set desired fx duration
	this->shakeTimeLeft = duration;

	this->lastShakeOffset.x = __PIXELS_TO_METERS(4);

	// discard pending messages from previously started fx
	MessageDispatcher::discardDelayedMessagesFromSender(MessageDispatcher::getInstance(), Object::safeCast(this), kShake);

	// instantly send message to self to start fx
	MessageDispatcher::dispatchMessage(0, Object::safeCast(this), Object::safeCast(this), kShake, NULL);
}

// stop shaking the _camera
void CustomCameraEffectManager::fxShakeStop()
{
	this->shakeTimeLeft = 0;
}

// shake the _camera
void CustomCameraEffectManager::onScreenShake()
{
	// stop if no shaking time left
	if(this->shakeTimeLeft == 0)
	{
		// if needed, undo last offset
		if(this->lastShakeOffset.x != 0 || this->lastShakeOffset.y != 0)
		{
			this->lastShakeOffset.x = 0;
		}

		return;
	}

	long seed = Utilities::randomSeed();

	int nextShakeDelay = MINIMUM_SHAKE_DELAY + Utilities::random(seed, __ABS(SHAKE_DELAY_DELTA));

	// subtract time until next shake
	this->shakeTimeLeft = (this->shakeTimeLeft <= nextShakeDelay) ? 0 : this->shakeTimeLeft - nextShakeDelay;

	// new offset
	this->lastShakeOffset.x = -this->lastShakeOffset.x;

	// move screen a bit
	Camera::move(_camera, this->lastShakeOffset, false);

	// send message for next screen movement
	MessageDispatcher::dispatchMessage(nextShakeDelay, Object::safeCast(this), Object::safeCast(this), kShake, NULL);
}