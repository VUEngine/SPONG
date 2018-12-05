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

#include <string.h>

#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <Languages.h>
#include <PhysicalWorld.h>
#include <PongState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeypadManager.h>
#include <Player.h>
#include <GameEvents.h>
#include <debugUtilities.h>
#include <AutoPauseManager.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec PLAYFIELD_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void PongState::constructor()
{
	Base::constructor();

	this->isVersusMode = false;
}

void PongState::destructor()
{
	// destroy base
	Base::destructor();
}

void PongState::setVersusMode(bool value)
{
	this->isVersusMode = value;
}

bool PongState::getVersusMode()
{
	return this->isVersusMode;
}

bool PongState::isVersusMode()
{
	return PongState::getVersusMode(this);
}

void PongState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&PLAYFIELD_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	Player::getReady(Player::getInstance(), GameState::safeCast(this));

	// disable automatic pause in versus mode
	AutoPauseManager::setActive(AutoPauseManager::getInstance(), !this->isVersusMode);

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void PongState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

void PongState::processUserInput(UserInput userInput)
{
	if(userInput.pressedKey)
	{
		if(K_SEL & userInput.pressedKey)
		{
			// adjustment screen
//				PlatformerLevelState::setModeToPaused(this);

			// set next state of adjustment screen state to null so it can differentiate between
			// being called the splash screen sequence or from within the game (a bit hacky...)
//				SplashScreenState::setNextState(SplashScreenState::safeCast(AdjustmentScreenState::getInstance()), NULL);

			// pause game and switch to adjustment screen state
//				Game::pause(Game::getInstance(), GameState::safeCast(AdjustmentScreenState::getInstance()));

			return;
		}
		else if(K_STA & userInput.pressedKey)
		{
			// pause game and switch to pause screen state
//				Game::pause(Game::getInstance(), GameState::safeCast(PauseScreenState::getInstance()));

			return;
		}
	}

	if(this->isVersusMode)
	{
	//	PongBall pongBall = Player::getPongBall(Player::getInstance());
		DataToTransmit dataToTransmit;
	//	dataToTransmit.ballLocalPosition = *Container::getLocalPosition(Container::safeCast(pongBall));
		dataToTransmit.resumedUserInput.pressedKey = userInput.pressedKey;
		dataToTransmit.resumedUserInput.releasedKey = userInput.releasedKey;
		dataToTransmit.resumedUserInput.holdKey = userInput.holdKey;

		CommunicationManager::sendAndReceiveData(CommunicationManager::getInstance(), (BYTE*)&dataToTransmit, (BYTE*)&this->opponentData, sizeof(dataToTransmit));
/*
		if(kPlayerTwo == Player::getPlayerNumber(Player::getInstance()))
		{
			Container::setLocalPosition(Container::safeCast(pongBall), &this->opponentData.ballLocalPosition);
		}
*/
	//	CommunicationManager::sendAndReceiveDataAsync(CommunicationManager::getInstance(), (BYTE*)&dataToTransmit, sizeof(dataToTransmit), (EventListener)PongState::onUserInputTransmitted, Object::safeCast(this));
	}

	Object::fireEvent(this, kEventUserInput);
}

bool PongState::processUserInputRegardlessOfInput()
{
	return true;
}

void PongState::onUserInputTransmitted(Object eventFirer __attribute__ ((unused)))
{
	this->opponentData = *((DataToTransmit*)CommunicationManager::getData(CommunicationManager::getInstance()));
/*
	if(kPlayerTwo == Player::getPlayerNumber(Player::getInstance()))
	{
		PongBall pongBall = Player::getPongBall(Player::getInstance());
		Container::setLocalPosition(Container::safeCast(pongBall), &this->opponentData.ballLocalPosition);
	}
	*/
}

void PongState::switchState()
{
	Base::switchState(this);

	Player::gameIsOver(Player::getInstance(), GameState::safeCast(this));
}

ResumedUserInput PongState::getOpponentInput()
{
	return this->opponentData.resumedUserInput;
}

