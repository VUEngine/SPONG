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

#include <VUEngine.h>
#include <AnimatedEntity.h>
#include <Camera.h>
#include <MessageDispatcher.h>
#include <I18n.h>
#include <LangSelectScreenState.h>
#include <TitleScreenState.h>
#include <Languages.h>
#include <KeypadManager.h>
#include <Utilities.h>
#include <ProgressManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec LangSelectScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void LangSelectScreenState::constructor()
{
	Base::constructor();

	// init members
	this->language = 0;
	SplashScreenState::setNextState(SplashScreenState::safeCast(this), GameState::safeCast(TitleScreenState::getInstance()));
	this->stageSpec = (StageSpec*)&LangSelectScreenStage;
}

void LangSelectScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void LangSelectScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	this->language = I18n::getActiveLanguage(I18n::getInstance());
	LangSelectScreenState::print(this);
}

void LangSelectScreenState::changeLanguage(bool forward)
{
	int numLangs = sizeof(I18n::getLanguages(I18n::getInstance()));
	this->language = forward
		? (this->language < (numLangs - 1)) ? this->language + 1 : 0
		: (this->language > 0) ? this->language - 1 : numLangs - 1;
	I18n::setActiveLanguage(I18n::getInstance(), this->language);
	SaveDataManager::setLanguage(ProgressManager::getInstance(), this->language);
	LangSelectScreenState::print(this);
}

void LangSelectScreenState::processInput(uint32 pressedKey)
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
		SplashScreenState::loadNextState(SplashScreenState::safeCast(this));
	}
}

void LangSelectScreenState::print()
{
	// move cursor entity
	Entity cursorEntity = Entity::safeCast(Container::getChildByName(
		Container::safeCast(VUEngine::getStage(VUEngine::getInstance())),
		"Cursor",
		false
	));
	Vector3D cursorPosition = {__PIXELS_TO_METERS(120 + this->language * 48), __PIXELS_TO_METERS(96), 0};
	Entity::setLocalPosition(cursorEntity, &cursorPosition);

	// move cursor entity
	AnimatedEntity LangNameEntity = AnimatedEntity::safeCast(Container::getChildByName(
		Container::safeCast(VUEngine::getStage(VUEngine::getInstance())),
		"LangName",
		false
	));

	// change language name position
	Vector3D languageNamePosition =
	{
		__PIXELS_TO_METERS(132 + (this->language * 48)),
		__PIXELS_TO_METERS(124),
		0,
	};
	Entity::setLocalPosition(LangNameEntity, &languageNamePosition);
}

