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

#include <Game.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <Languages.h>
#include <PhysicalWorld.h>
#include <TitleScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeypadManager.h>
#include <SoundManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <PongState.h>
#include <HighscoresScreenState.h>
#include <OptionsScreenState.h>
#include <CreditsScreenState.h>
#include <BrightnessManager.h>
#include <GameEvents.h>
#include <CommunicationManager.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec TitleScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void TitleScreenState::constructor()
{
	Base::constructor();

	// init members
	this->entityPressStart = NULL;
	this->entityMainMenu = NULL;
	this->entityMainMenuVersus = NULL;
	this->entityCursor = NULL;
	this->entityWaitingForOtherPlayer = NULL;
	this->mode = kTitleScreenModeShowPressStart;
	this->option = 0;
}

void TitleScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void TitleScreenState::enter(void* owner)
{
	// reset mode
	this->mode = kTitleScreenModeShowPressStart;

	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&TitleScreenStage, NULL, true, false);

	// get entity references
	this->entityPressStart = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "PrssStrt", true));
	this->entityMainMenu = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "MainMenu", true));
	this->entityMainMenuVersus = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "MMVersus", true));
	this->entityCursor = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "MMCursor", true));
	this->entityWaitingForOtherPlayer = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "Waiting", true));

	// initial entity states
	Entity::hide(this->entityMainMenu);
	Entity::hide(this->entityWaitingForOtherPlayer);
	TitleScreenState::updateCursorPosition(this);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		TitleScreenState::enableVersusMode(this);
	}
	else
	{
		CommunicationManager::addEventListener(CommunicationManager::getInstance(), Object::safeCast(this), (EventListener)TitleScreenState::onConnectedWithRemoteSystem, kEventCommunicationsConnected);
	}

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void TitleScreenState::exit(void* owner)
{
	CommunicationManager::removeEventListener(CommunicationManager::getInstance(), Object::safeCast(this), (EventListener)TitleScreenState::onConnectedWithRemoteSystem, kEventCommunicationsConnected);

	this->entityPressStart = NULL;
	this->entityMainMenu = NULL;
	this->entityMainMenuVersus = NULL;
	this->entityCursor = NULL;
	this->entityWaitingForOtherPlayer = NULL;
	this->mode = kTitleScreenModeShowPressStart;
	this->option = 0;

	// call base
	Base::exit(this, owner);
}

void TitleScreenState::onConnectedWithRemoteSystem(Object eventFirer __attribute__ ((unused)))
{
	TitleScreenState::enableVersusMode(this);
}

void TitleScreenState::enableVersusMode()
{
	if(!isDeleted(this->entityMainMenuVersus))
	{
		VirtualList sprites = Entity::getSprites(this->entityMainMenuVersus);
		Sprite sprite = Sprite::safeCast(VirtualList::front(sprites));
		Texture texture = Sprite::getTexture(sprite);
		Texture::setPalette(texture, 0);
		Texture::rewrite(texture);
	}
}

void TitleScreenState::updateCursorPosition()
{
	// change position
	Vector3D position =
	{
		0,
		__PIXELS_TO_METERS((this->option * 12) - 24),
		0,
	};

	Entity::setLocalPosition(this->entityCursor, &position);

	// change parallax
	VirtualList cursorSprites = Entity::getSprites(this->entityCursor);
	PixelVector displacement = *Sprite::getDisplacement(VirtualList::front(cursorSprites));
	displacement.parallax = (this->option < 2) ? -1 : (this->option > 3) ? 1 : 0;
	Sprite::setDisplacement(VirtualList::front(cursorSprites), &displacement);
}

void TitleScreenState::processUserInputModePressStart(UserInput userInput)
{
	if(K_STA & userInput.pressedKey)
	{
		// remove blinking "press start button"
		Entity::hide(this->entityPressStart);

		// show options
		Entity::show(this->entityMainMenu);

		// set mode to showing options
		this->mode = kTitleScreenModeShowOptions;
	}
}

void TitleScreenState::processUserInputModeShowOptions(UserInput userInput)
{
	if((K_A | K_STA) & userInput.pressedKey)
	{
		// (temporarily) disable challenge mode and highscores screen
		if( this->option == kTitleScreenOptionChallengeMode ||
			this->option == kTitleScreenOptionHighscores)
		{
			return;
		}

		if(this->option == kTitleScreenOptionVersusMode && !CommunicationManager::isConnected(CommunicationManager::getInstance()))
		{
			return;
		}
		else
		{
			Entity::show(this->entityWaitingForOtherPlayer);
		}

		// disable user input
		Game::disableKeypad(Game::getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
		}
	}
	else if((K_LU | K_RU) & userInput.pressedKey)
	{
		this->option = (this->option > 0) ? this->option - 1 : kTitleScreenOptionCredits;
		TitleScreenState::updateCursorPosition(this);
	}
	else if((K_LD | K_RD) & userInput.pressedKey)
	{
		this->option = (this->option < kTitleScreenOptionCredits) ? this->option + 1 : 0;
		TitleScreenState::updateCursorPosition(this);
	}
}

void TitleScreenState::onStartMatchMessageSend(Object eventFirer __attribute__ ((unused)))
{
	uint32 message = *((uint32*)CommunicationManager::getReceivedMessage(CommunicationManager::getInstance()));

	if(kCommunicationMessageStartVersusMatch == message)
	{
		PongState::setVersusMode(PongState::getInstance(), true);

		// transition layer animation
		AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
		}
	}
	else
	{
		// try again communication sending a delayed message
		// not immediately!
	}
}

void TitleScreenState::processUserInput(UserInput userInput)
{
	switch(this->mode)
	{
		case kTitleScreenModeShowPressStart:
		{
			TitleScreenState::processUserInputModePressStart(this, userInput);
			break;
		}
		case kTitleScreenModeShowOptions:
		{
			TitleScreenState::processUserInputModeShowOptions(this, userInput);
			break;
		}
	}
}

void TitleScreenState::switchState()
{
	switch(this->option)
	{
		case kTitleScreenOptionMarathonMode:
		case kTitleScreenOptionChallengeMode:
		{
			BrightnessManager::hideScreen(BrightnessManager::getInstance());
			Game::changeState(Game::getInstance(), GameState::safeCast(PongState::getInstance()));
			break;
		}

		case kTitleScreenOptionHighscores:
		{
			BrightnessManager::hideScreen(BrightnessManager::getInstance());
			Game::changeState(Game::getInstance(), GameState::safeCast(HighscoresScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionOptions:
		{
			BrightnessManager::hideScreen(BrightnessManager::getInstance());
			Game::changeState(Game::getInstance(), GameState::safeCast(OptionsScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionCredits:
		{
			BrightnessManager::hideScreen(BrightnessManager::getInstance());
			Game::changeState(Game::getInstance(), GameState::safeCast(CreditsScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionVersusMode:
		{
			// disable user input
			Game::disableKeypad(Game::getInstance());

			// stop all sounds
			SoundManager::stopAllSounds(SoundManager::getInstance(), true);
			SoundManager::reset(SoundManager::getInstance());

			if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
			{
				uint32 sentMessage = kCommunicationMessageStartVersusMatch;
				uint32 receivedMessage = sentMessage + 1;
				uint32 dummyData = 0xFAB12;
				uint32 receivedData = dummyData + 1;

				do
				{
					CommunicationManager::sendAndReceiveData(CommunicationManager::getInstance(), sentMessage, (BYTE*)&dummyData, sizeof(dummyData));

					receivedMessage = CommunicationManager::getReceivedMessage(CommunicationManager::getInstance());
					receivedData = *(uint32*)CommunicationManager::getReceivedData(CommunicationManager::getInstance());
				}
				while(receivedMessage != sentMessage && receivedData != dummyData);

				PongState::setVersusMode(PongState::getInstance(), true);

				BrightnessManager::hideScreen(BrightnessManager::getInstance());
				Game::changeState(Game::getInstance(), GameState::safeCast(PongState::getInstance()));
			}
			break;
		}
	}
}
