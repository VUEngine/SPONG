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
#include <BrightnessManager.h>
#include <VirtualList.h>
#include <CameraEffectManager.h>
#include <VIPManager.h>
#include <MessageDispatcher.h>
#include <GameEvents.h>
#include <macros.h>


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void BrightnessManager::constructor()
{
	// construct base object
	Base::constructor();

	// init members
	this->brightnessFactor = DEFAULT_BRIGHTNESS_FACTOR;
}

void BrightnessManager::destructor()
{
	// destroy base
	Base::destructor();
}

void BrightnessManager::delayedShowScreen()
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	// show screen in a moment
	MessageDispatcher::dispatchMessage(50, Object::safeCast(this), Object::safeCast(this), kMessageShowScreen, NULL);
}

void BrightnessManager::showScreen()
{
	BrightnessManager::setDefaultBrightness(this);
	Object::fireEvent(Game::getCurrentState(Game::getInstance()), kEventShowScreen);
}

void BrightnessManager::hideScreen()
{
	VIPManager::lowerBrightness(VIPManager::getInstance());
}

void BrightnessManager::setDefaultBrightness()
{
	Brightness defaultBrightness = CameraEffectManager::getDefaultBrightness(CameraEffectManager::getInstance());

	int darkRed = (0 >= (darkRed = (defaultBrightness.darkRed + (this->brightnessFactor * 8)))) ? 4 : darkRed;
	int mediumRed = (0 >= (mediumRed = (defaultBrightness.mediumRed + (this->brightnessFactor * 16)))) ? 8 : mediumRed;
	int brightRed = (0 >= (brightRed = (defaultBrightness.brightRed + (this->brightnessFactor * 32)))) ? 16 : brightRed;

	Brightness brightness = 
	{
		darkRed,
		mediumRed,
		brightRed
	};

	VIPManager::setupBrightness(VIPManager::getInstance(), &brightness);
}

void BrightnessManager::setBrightnessFactor(int8 brightnessFactor)
{
	this->brightnessFactor = brightnessFactor;
	BrightnessManager::setDefaultBrightness(this);
}

int8 BrightnessManager::getBrightnessFactor()
{
	return this->brightnessFactor;
}

bool BrightnessManager::handleMessage(Telegram telegram)
{
	switch(Telegram::getMessage(telegram))
	{
		case kMessageShowScreen:
			// show screen
			BrightnessManager::showScreen(this);
			break;
	}
	return true;
}

