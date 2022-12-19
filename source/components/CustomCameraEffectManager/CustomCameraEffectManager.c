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

#include <CustomCameraEffectManager.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <Actor.h>
#include <VUEngine.h>
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
void CustomCameraEffectManager::fxShakeStart(uint16 duration)
{
	// set desired fx duration
	this->shakeTimeLeft = duration;

	this->lastShakeOffset.x = __PIXELS_TO_METERS(4);

	// discard pending messages from previously started fx
	MessageDispatcher::discardDelayedMessagesFromSender(MessageDispatcher::getInstance(), ListenerObject::safeCast(this), kShake);

	// instantly send message to self to start fx
	MessageDispatcher::dispatchMessage(0, ListenerObject::safeCast(this), ListenerObject::safeCast(this), kShake, NULL);
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
    Camera::translate(_camera, this->lastShakeOffset, false);

	// send message for next screen movement
	MessageDispatcher::dispatchMessage(nextShakeDelay, ListenerObject::safeCast(this), ListenerObject::safeCast(this), kShake, NULL);
}
