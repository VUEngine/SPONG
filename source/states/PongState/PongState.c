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
#include <KeyPadManager.h>
#include <Player.h>
#include <GameEvents.h>
#include <debugUtilities.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef PLAYFIELD_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

// class's constructor
void PongState::constructor()
{
	Base::constructor();

	// add event listeners
	Object::addEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)PongState_onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void PongState::destructor()
{
	// remove event listeners
	Object::removeEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)PongState_onTransitionOutComplete, kEventTransitionOutComplete);

	// destroy base
	Base::destructor();
}

// state's enter
void PongState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageDefinition*)&PLAYFIELD_STAGE_ST, NULL, true);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// enable user input
	Game::enableKeypad(Game::getInstance());

	Player::getReady(Player::getInstance(), GameState::safeCast(this));

	// show screen
	BrightnessManager::showScreen(BrightnessManager::getInstance());
}

void PongState::execute(void* owner)
{
	Base::execute(this, owner);
	UserInput userInput = KeypadManager::getUserInput(KeypadManager::getInstance());
return;
	if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
	{
		if(CommunicationManager::isMaster(CommunicationManager::getInstance()))
			PRINT_TEXT("master", 30, 6);
		else
			PRINT_TEXT("slave", 30, 6);


		if(CommunicationManager::isMaster(CommunicationManager::getInstance()))
		{
		PRINT_TEXT("master", 30, 7);
//			static u8 pressedKey = 0x12;
//			static u16 pressedKey = 0x1234;
//			CommunicationManager::sendData(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey));
//			CommunicationManager::sendDataAsync(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey), PongState::onCommunicationCompletes, Object::safeCast(this));
			//pressedKey++;
		}
		else
		{
		PRINT_TEXT("slave", 30, 7);
		//	static u32 expectedPressedKey = 0x12345678;
//			u32 pressedKey = 0;
//			static u16 expectedPressedKey = 0x1234;
//			u16 pressedKey = 0;
//			CommunicationManager::receiveData(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey), PongState::onCommunicationCompletes);
//			CommunicationManager::receiveDataAsync(CommunicationManager::getInstance(), sizeof(pressedKey), PongState::onCommunicationCompletes, Object::safeCast(this));

/*
//			PRINT_HEX(pressedKey, 5, 10);
			//expectedPressedKey++;

			static int success = 0;
			static int errors = 0;

			if(pressedKey == expectedPressedKey)
			{
				success++;
			}
			else
			{
				errors++;
			}

			PRINT_TEXT("     %", 2, 5);
			PRINT_FLOAT(success / (float)(success + errors), 2, 5);
			PRINT_INT(success, 10, 5);
			PRINT_INT(errors, 16, 5);
			*/
		}
	}
}

void PongState::onCommunicationCompletes(Object eventFirer)
{
		if(CommunicationManager::isMaster(CommunicationManager::getInstance()))
		{
			PRINT_TIME(30, 8);
		PRINT_TEXT("master event", 30, 7);
		}
		else
		{
		PRINT_TEXT("slave event", 30, 7);
			PRINT_TIME(30, 8);
			BYTE* pressedKeyPointer = CommunicationManager::getData(CommunicationManager::getInstance());
//			static u8 expectedPressedKey = 0x12;
//			u8 pressedKey = *((u8*)pressedKeyPointer);
//			static u16 expectedPressedKey = 0x1234;
//			u16 pressedKey = *((u16*)pressedKeyPointer);
			static u32 expectedPressedKey = 0x12345678;
			u32 pressedKey = *((u32*)pressedKeyPointer);

				PRINT_HEX(pressedKey, 10, 16);

//			PRINT_HEX(pressedKey, 5, 10);
			//expectedPressedKey++;

			static int success = 0;
			static int errors = 0;

			if(pressedKey == expectedPressedKey)
			{
				success++;
			}
			else
			{
				errors++;
			}

			PRINT_TEXT("     %", 2, 5);
			PRINT_FLOAT(success / (float)(success + errors), 2, 5);
			PRINT_INT(success, 10, 5);
			PRINT_INT(errors, 16, 5);

		}
}

// state's exit
void PongState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

// state's resume
void PongState::resume(void* owner)
{
	Base::resume(this, owner);

	Camera::startEffect(Camera::getInstance(), kFadeIn, __FADE_DELAY);
}

// state's suspend
void PongState::suspend(void* owner)
{
	Camera::startEffect(Camera::getInstance(), kFadeOut, __FADE_DELAY);

	Base::suspend(this, owner);
}


void PongState::processUserInput(UserInput userInput)
{
	if(userInput.pressedKey & ~K_PWR)
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

		Object::fireEvent(Object::safeCast(this), kEventUserInput);

	/*
		// disable user input
		Game::disableKeypad(Game::getInstance());

		// transition layer animation
		AnimatedEntity transitionLayerEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "TRNSLYR", true));
		if(transitionLayerEntity)
		{
			AnimatedEntity::playAnimation(transitionLayerEntity, "FadeOut");
		}
	*/
	}

//	Object::fireEvent(Object::safeCast(this), kEventUserInput);

	if(userInput.pressedKey & K_A)
	{
		if(CommunicationManager::isConnected(CommunicationManager::getInstance()))
		{
			PRINT_TEXT("connected", 30, 6);

//				static u8 pressedKey = 0x12;
//			static u16 pressedKey = 0x1234;
				static u32 pressedKey = 0x12345678;

			if(CommunicationManager::isMaster(CommunicationManager::getInstance()))
			{
			PRINT_TEXT("send as master", 30, 7);
//				CommunicationManager::sendData(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey));

			CommunicationManager::sendDataAsync(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey), (EventListener)PongState::onCommunicationCompletes, Object::safeCast(this));
				//pressedKey++;
			}
			else
			{
			PRINT_TEXT("receive as slave", 30, 7);
				static u8 expectedPressedKey = 0x12;
//				u8 pressedKey = 0;
//				static u16 expectedPressedKey = 0x1234;
//				u16 pressedKey = 0;
//				static u32 expectedPressedKey = 0x12345678;
//				u32 pressedKey = 0;
//				CommunicationManager::receiveData(CommunicationManager::getInstance(), (BYTE*)&pressedKey, sizeof(pressedKey));
			//PRINT_TIME(1, 6); PRINT_TEXT("AA", 10, 6);
			CommunicationManager::receiveDataAsync(CommunicationManager::getInstance(), sizeof(pressedKey), (EventListener)PongState::onCommunicationCompletes, Object::safeCast(this));
return;
				//expectedPressedKey++;

				static int success = 0;
				static int errors = 0;

				if(pressedKey == expectedPressedKey)
				{
				PRINT_HEX(pressedKey, 25, 15);
					success++;
				}
				else
				{
				PRINT_HEX(pressedKey, 25, 17);
					errors++;
				}

				PRINT_TEXT("     %", 2, 5);
				PRINT_FLOAT(success / (float)(success + errors), 2, 5);
				PRINT_INT(success, 10, 5);
				PRINT_INT(errors, 16, 5);
			}
		}
	}
}

// handle event
void PongState::onTransitionOutComplete(Object eventFirer __attribute__ ((unused)))
{
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	Player::gameIsOver(Player::getInstance(), GameState::safeCast(this));
}
