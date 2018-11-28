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
#include <TitleScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeyPadManager.h>
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

extern StageROMDef TITLE_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
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

	// add event listeners
	Object::addEventListener(this, Object::safeCast(this), (EventListener)TitleScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void TitleScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(this, Object::safeCast(this), (EventListener)TitleScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	Base::destructor();
}

// state's enter
void TitleScreenState::enter(void* owner)
{
	// reset mode
	this->mode = kTitleScreenModeShowPressStart;

	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageDefinition*)&TITLE_SCREEN_STAGE_ST, NULL, true);

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

	// enable user input
	Game::enableKeypad(Game::getInstance());

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

// state's exit
void TitleScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

// state's resume
void TitleScreenState::resume(void* owner)
{
	Base::resume(this, owner);

	Camera::startEffect(Camera::getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
void TitleScreenState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}

void TitleScreenState::execute(void* owner __attribute__ ((unused)))
{
	Base::execute(this, owner);

	// poll comms to enable/disable "versus mode" option accordingly
	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
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
	PixelVector displacement = Sprite::getDisplacement(VirtualList::front(cursorSprites));
	displacement.parallax = (this->option < 2) ? -1 : (this->option > 3) ? 1 : 0;
	Sprite::setDisplacement(VirtualList::front(cursorSprites), displacement);
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
	u32 message = *((u32*)CommunicationManager::getData(CommunicationManager::getInstance()));

	if(START_VERSUS_MATCH == message)
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

// handle event
void TitleScreenState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	switch(this->option)
	{
		case kTitleScreenOptionMarathonMode:
		case kTitleScreenOptionChallengeMode:
		{
			Game::changeState(Game::getInstance(), GameState::safeCast(PongState::getInstance()));
			break;
		}

		case kTitleScreenOptionHighscores:
		{
			Game::changeState(Game::getInstance(), GameState::safeCast(HighscoresScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionOptions:
		{
			Game::changeState(Game::getInstance(), GameState::safeCast(OptionsScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionCredits:
		{
			Game::changeState(Game::getInstance(), GameState::safeCast(CreditsScreenState::getInstance()));
			break;
		}

		case kTitleScreenOptionVersusMode:
		{
			if(this->option == kTitleScreenOptionVersusMode)
			{
				// disable user input
				Game::disableKeypad(Game::getInstance());

				// stop all sounds
				SoundManager::stopAllSound(SoundManager::getInstance());
				SoundManager::reset(SoundManager::getInstance());

				Entity::show(this->entityWaitingForOtherPlayer);

				if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
				{

					u32 messageToSend = START_VERSUS_MATCH;
					u32 receivedMessage = START_VERSUS_MATCH;

					do
					{
						CommunicationManager::sendAndReceiveData(CommunicationManager::getInstance(), (BYTE*)&messageToSend, (BYTE*)&receivedMessage, sizeof(receivedMessage));
					}
					while(START_VERSUS_MATCH != receivedMessage);

					PongState::setVersusMode(PongState::getInstance(), true);

					Game::changeState(Game::getInstance(), GameState::safeCast(PongState::getInstance()));

					//CommunicationManager::sendAndReceiveDataAsync(CommunicationManager::getInstance(), (BYTE*)&message, sizeof(message), (EventListener)TitleScreenState::onStartMatchMessageSend, Object::safeCast(this));
				}
			}
			break;
		}
	}
}
