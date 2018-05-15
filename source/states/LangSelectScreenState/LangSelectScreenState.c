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
#include <AnimatedEntity.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <LangSelectScreenState.h>
#include <TitleScreenState.h>
#include <Languages.h>
#include <KeyPadManager.h>
#include <Utilities.h>
#include <ProgressManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef LANG_SELECT_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------


void LangSelectScreenState::constructor(LangSelectScreenState this);
void LangSelectScreenState::changeLanguage(LangSelectScreenState this, bool forward);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------





//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void __attribute__ ((noinline)) LangSelectScreenState::constructor(LangSelectScreenState this)
{
	ASSERT(this, "LangSelectScreenState::constructor: null this");

	Base::constructor();

	// init members
	this->language = 0;
	SplashScreenState::setNextState(__SAFE_CAST(SplashScreenState, this), __SAFE_CAST(GameState, TitleScreenState::getInstance()));
	this->stageDefinition = (StageDefinition*)&LANG_SELECT_SCREEN_STAGE_ST;
}

void LangSelectScreenState::destructor(LangSelectScreenState this)
{
	ASSERT(this, "LangSelectScreenState::destructor: null this");

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void LangSelectScreenState::enter(LangSelectScreenState this, void* owner)
{
	// call base
	Base::enter(this, owner);

	this->language = I18n::getActiveLanguage(I18n::getInstance());
	LangSelectScreenState::print(this);
}

void LangSelectScreenState::changeLanguage(LangSelectScreenState this, bool forward)
{
	int numLangs = sizeof(I18n::getLanguages(I18n::getInstance()));
	this->language = forward
		? (this->language < (numLangs - 1)) ? this->language + 1 : 0
		: (this->language > 0) ? this->language - 1 : numLangs - 1;
	I18n::setActiveLanguage(I18n::getInstance(), this->language);
	ProgressManager::setLanguage(ProgressManager::getInstance(), this->language);
	LangSelectScreenState::print(this);
}

void LangSelectScreenState::processInput(LangSelectScreenState this, u32 pressedKey)
{
	if((pressedKey & K_LL) || (pressedKey & K_RL))
	{
		LangSelectScreenState::changeLanguage(this, false);
	}
	else if((pressedKey & K_LR) || (pressedKey & K_RR))
	{
		LangSelectScreenState::changeLanguage(this, true);
	}
	else if((pressedKey & K_A) || (pressedKey & K_STA))
	{
		SplashScreenState::loadNextState(__SAFE_CAST(SplashScreenState, this));
	}
}

void LangSelectScreenState::print(LangSelectScreenState this __attribute__ ((unused)))
{
	// move cursor entity
	Entity cursorEntity = __SAFE_CAST(Entity, Container::getChildByName(
		__SAFE_CAST(Container, Game::getStage(Game::getInstance())),
		"Cursor",
		false
	));
	Vector3D cursorPosition = {__PIXELS_TO_METERS(120 + this->language * 48), __PIXELS_TO_METERS(96), 0};
	Entity::setLocalPosition(cursorEntity, &cursorPosition);

	// move cursor entity
	AnimatedEntity LangNameEntity = __SAFE_CAST(AnimatedEntity, Container::getChildByName(
		__SAFE_CAST(Container, Game::getStage(Game::getInstance())),
		"LangName",
		false
	));

	// change language name
	Object::fireEvent(__SAFE_CAST(Object, this), kEventLanguageChanged);

	// change language name position
	Vector3D languageNamePosition =
	{
		__PIXELS_TO_METERS(132 + (this->language * 48)),
		__PIXELS_TO_METERS(124),
		0,
	};
	Entity::setLocalPosition(__SAFE_CAST(Entity, LangNameEntity), &languageNamePosition);
}

