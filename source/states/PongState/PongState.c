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

#include <string.h>

#include <VUEngine.h>
#include <Utilities.h>
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
#include <AutomaticPauseManager.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec PlayfieldStage;


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
	if(PongState::isVersusMode(this))
	{
		// Reset random seed in multiplayer mode so both machines are completely in sync
		Utilities::resetRandomSeed();
	}

	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&PlayfieldStage, NULL, true, false);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	Player::getReady(Player::getInstance(), GameState::safeCast(this));

	// disable automatic pause in versus mode
	AutomaticPauseManager::setActive(AutomaticPauseManager::getInstance(), !this->isVersusMode);

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
//				VUEngine::pause(VUEngine::getInstance(), GameState::safeCast(AdjustmentScreenState::getInstance()));

			return;
		}
		else if(K_STA & userInput.pressedKey)
		{
			// pause game and switch to pause screen state
//				VUEngine::pause(VUEngine::getInstance(), GameState::safeCast(PauseScreenState::getInstance()));

			return;
		}
	}

	if(this->isVersusMode)
	{
		this->opponentData.resumedUserInput.pressedKey = userInput.pressedKey;
		this->opponentData.resumedUserInput.releasedKey = userInput.releasedKey;
		this->opponentData.resumedUserInput.holdKey = userInput.holdKey;

		uint32 sentMessage = kCommunicationMessageSendAndReceiveInput;
		uint32 receivedMessage = sentMessage + 1;

		do
		{
			CommunicationManager::sendAndReceiveData(CommunicationManager::getInstance(), sentMessage, (BYTE*)&this->opponentData, sizeof(this->opponentData));

			receivedMessage = CommunicationManager::getReceivedMessage(CommunicationManager::getInstance());
		}
		while(receivedMessage != sentMessage);

		this->opponentData = *(DataToTransmit*)CommunicationManager::getReceivedData(CommunicationManager::getInstance());

/*
		if(kPlayerTwo == Player::getPlayerNumber(Player::getInstance()))
		{
			Container::setLocalPosition(Container::safeCast(pongBall), &this->opponentData.ballLocalPosition);
		}
*/
	//	CommunicationManager::sendAndReceiveDataAsync(CommunicationManager::getInstance(), (BYTE*)&dataToTransmit, sizeof(dataToTransmit), (EventListener)PongState::onUserInputTransmitted, Object::safeCast(this));
	}

	ListenerObject::fireEvent(this, kEventUserInput);
}

bool PongState::processUserInputRegardlessOfInput()
{
	return true;
}

void PongState::onUserInputTransmitted(ListenerObject eventFirer __attribute__ ((unused)))
{
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

