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
#include <EventManager.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMDef LANG_SELECT_SCREEN_STAGE_ST;


//---------------------------------------------------------------------------------------------------------
//												PROTOTYPES
//---------------------------------------------------------------------------------------------------------

static void LangSelectScreenState_destructor(LangSelectScreenState this);
static void LangSelectScreenState_constructor(LangSelectScreenState this);
static void LangSelectScreenState_processInput(LangSelectScreenState this, u32 pressedKey);
void LangSelectScreenState_changeLanguage(LangSelectScreenState this, bool forward);
static void LangSelectScreenState_print(LangSelectScreenState this);


//---------------------------------------------------------------------------------------------------------
//											CLASS'S DEFINITION
//---------------------------------------------------------------------------------------------------------

__CLASS_DEFINITION(LangSelectScreenState, SplashScreenState);
__SINGLETON_DYNAMIC(LangSelectScreenState);


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

static void __attribute__ ((noinline)) LangSelectScreenState_constructor(LangSelectScreenState this)
{
	ASSERT(this, "LangSelectScreenState::constructor: null this");

	__CONSTRUCT_BASE(SplashScreenState);

	// init members
	this->language = 0;
	SplashScreenState_setNextState(__SAFE_CAST(SplashScreenState, this), __SAFE_CAST(GameState, TitleScreenState_getInstance()));
	this->stageDefinition = (StageDefinition*)&LANG_SELECT_SCREEN_STAGE_ST;
}

static void LangSelectScreenState_destructor(LangSelectScreenState this)
{
	ASSERT(this, "LangSelectScreenState::destructor: null this");

	// destroy base
	__SINGLETON_DESTROY;
}

// state's enter
void LangSelectScreenState_enter(LangSelectScreenState this, void* owner)
{
	// call base
	__CALL_BASE_METHOD(SplashScreenState, enter, this, owner);

	this->language = I18n_getActiveLanguage(I18n_getInstance());
	LangSelectScreenState_print(this);
}

void LangSelectScreenState_changeLanguage(LangSelectScreenState this, bool forward)
{
	int numLangs = sizeof(I18n_getLanguages(I18n_getInstance()));
	this->language = forward
		? (this->language < (numLangs - 1)) ? this->language + 1 : 0
		: (this->language > 0) ? this->language - 1 : numLangs - 1;
	I18n_setActiveLanguage(I18n_getInstance(), this->language);
	ProgressManager_setLanguage(ProgressManager_getInstance(), this->language);
	LangSelectScreenState_print(this);
}

void LangSelectScreenState_processInput(LangSelectScreenState this, u32 pressedKey)
{
	if((pressedKey & K_LL) || (pressedKey & K_RL))
	{
		LangSelectScreenState_changeLanguage(this, false);
	}
	else if((pressedKey & K_LR) || (pressedKey & K_RR))
	{
		LangSelectScreenState_changeLanguage(this, true);
	}
	else if((pressedKey & K_A) || (pressedKey & K_STA))
	{
		SplashScreenState_loadNextState(__SAFE_CAST(SplashScreenState, this));
	}
}

static void LangSelectScreenState_print(LangSelectScreenState this __attribute__ ((unused)))
{
	// move cursor entity
	Entity cursorEntity = __SAFE_CAST(Entity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"Cursor",
		false
	));
	Vector3D cursorPosition = {__PIXELS_TO_METERS(120 + this->language * 48), __PIXELS_TO_METERS(96), 0};
	Entity_setLocalPosition(cursorEntity, &cursorPosition);

	// move cursor entity
	AnimatedEntity LangNameEntity = __SAFE_CAST(AnimatedEntity, Container_getChildByName(
		__SAFE_CAST(Container, Game_getStage(Game_getInstance())),
		"LangName",
		false
	));

	// change language name
	Object_fireEvent(__SAFE_CAST(Object, EventManager_getInstance()), kEventLanguageChanged);

	// change language name position
	Vector3D languageNamePosition =
	{
		__PIXELS_TO_METERS(132 + (this->language * 48)),
		__PIXELS_TO_METERS(124),
		0,
	};
	Entity_setLocalPosition(__SAFE_CAST(Entity, LangNameEntity), &languageNamePosition);
}

