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
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <PongState.h>
#include <HighscoresScreenState.h>
#include <OptionsScreenState.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


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
	this->entityCursor = NULL;
	this->mode = kTitleScreenModeShowPressStart;
	this->option = 0;

	// add event listeners
	Object::addEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)TitleScreenState::onTransitionOutComplete, kEventTransitionOutComplete);
}

// class's destructor
void TitleScreenState::destructor()
{
	// remove event listeners
	Object::removeEventListener(Object::safeCast(this), Object::safeCast(this), (EventListener)TitleScreenState::onTransitionOutComplete, kEventTransitionOutComplete);

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
	this->entityCursor = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "MMCursor", true));

	// initial entity states
	Entity::hide(Entity::safeCast(this->entityMainMenu));
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
	displacement.parallax = (this->option - 2);
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
		this->option = (this->option > 0) ? this->option - 1 : kTitleScreenOptionOptions;
		TitleScreenState::updateCursorPosition(this);
	}
	else if((K_LD | K_RD) & userInput.pressedKey)
	{
		this->option = (this->option < kTitleScreenOptionOptions) ? this->option + 1 : 0;
		TitleScreenState::updateCursorPosition(this);
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
	// hide screen
	BrightnessManager::hideScreen(BrightnessManager::getInstance());

	switch(this->option)
	{
		case kTitleScreenOptionMarathonMode:
		case kTitleScreenOptionChallengeMode:
		case kTitleScreenOptionVersusMode:
			Game::changeState(Game::getInstance(), GameState::safeCast(PongState::getInstance()));
			break;

		case kTitleScreenOptionHighscores:
			Game::changeState(Game::getInstance(), GameState::safeCast(HighscoresScreenState::getInstance()));
			break;

		case kTitleScreenOptionOptions:
			Game::changeState(Game::getInstance(), GameState::safeCast(OptionsScreenState::getInstance()));
			break;
	}
}
