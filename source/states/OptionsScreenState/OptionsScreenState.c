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
#include <OptionsScreenState.h>
#include <ParticleSystem.h>
#include <ProgressManager.h>
#include <KeypadManager.h>
#include <AnimatedEntity.h>
#include <Utilities.h>
#include <TitleScreenState.h>
#include <AutomaticPauseManager.h>
#include <BrightnessManager.h>
#include <GameEvents.h>


//---------------------------------------------------------------------------------------------------------
//												DECLARATIONS
//---------------------------------------------------------------------------------------------------------

extern StageROMSpec OptionsScreenStage;


//---------------------------------------------------------------------------------------------------------
//												CLASS'S METHODS
//---------------------------------------------------------------------------------------------------------

void OptionsScreenState::constructor()
{
	Base::constructor();

	// init members
	this->entityCursor = NULL;
	this->mode = kOptionsScreenModeShowOptions;
	this->option = 0;
}

void OptionsScreenState::destructor()
{
	// destroy base
	Base::destructor();
}

void OptionsScreenState::enter(void* owner)
{
	// call base
	Base::enter(this, owner);

	// reset mode
	this->mode = kOptionsScreenModeShowOptions;

	// load stage
	GameState::loadStage(GameState::safeCast(this), (StageSpec*)&OptionsScreenStage, NULL, true, false);

	// get entity references
	this->entityCursor = Entity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "Cursor", true));

	// initial entity states
	OptionsScreenState::updateCursorPosition(this);
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
	OptionsScreenState::updateBrightnessMeter(this);

	// start clocks to start animations
	GameState::startClocks(GameState::safeCast(this));

	// show screen
	BrightnessManager::delayedShowScreen(BrightnessManager::getInstance());
}

void OptionsScreenState::exit(void* owner)
{
	// call base
	Base::exit(this, owner);
}

void OptionsScreenState::updateCursorPosition()
{
	Vector3D position =
	{
		__PIXELS_TO_METERS(234),
		__PIXELS_TO_METERS(100 + (this->option * 12)),
		0,
	};
	Entity::setLocalPosition(this->entityCursor, &position);
}

void OptionsScreenState::switchLanguage(bool forward)
{
	// update language
	int numLangs = sizeof(I18n::getLanguages(I18n::getInstance()));
	int language = I18n::getActiveLanguage(I18n::getInstance());
	language = forward
		? (language < (numLangs - 1)) ? language + 1 : 0
		: (language > 0) ? language - 1 : numLangs - 1;
	I18n::setActiveLanguage(I18n::getInstance(), language);
	SaveDataManager::setLanguage(ProgressManager::getInstance(), language);
}

void OptionsScreenState::updateAutomaticPauseCheckBox()
{
	bool AutomaticPauseEnabled = AutomaticPauseManager::isActive(AutomaticPauseManager::getInstance());
	AnimatedEntity AutomaticPauseCheckBoxEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "APChckBx", true));
	if(AutomaticPauseCheckBoxEntity)
	{
		AnimatedEntity::playAnimation(AutomaticPauseCheckBoxEntity, Utilities::itoa(AutomaticPauseEnabled, 10, 1));
	}
}

void OptionsScreenState::updateBrightnessMeter()
{
	int8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	AnimatedEntity brightnessMeterEntity = AnimatedEntity::safeCast(Container::getChildByName(Container::safeCast(Game::getStage(Game::getInstance())), "BrghtnMt", true));
	if(brightnessMeterEntity)
	{
		char* charBrightness = Utilities::itoa(brightnessFactor + 2, 10, 1);
		AnimatedEntity::playAnimation(brightnessMeterEntity, charBrightness);
	}
}

void OptionsScreenState::switchBrightness(bool forward)
{
	// change brightness factor
	int8 brightnessFactor = BrightnessManager::getBrightnessFactor(BrightnessManager::getInstance());
	brightnessFactor = forward
			? (brightnessFactor < 2) ? brightnessFactor + 1 : 2
			: (brightnessFactor > -2) ? brightnessFactor - 1 : -2;
	BrightnessManager::setBrightnessFactor(BrightnessManager::getInstance(), brightnessFactor);
	ProgressManager::setBrightnessFactor(ProgressManager::getInstance(), brightnessFactor);

	// update visual representation
	OptionsScreenState::updateBrightnessMeter(this);
}

void OptionsScreenState::toggleAutomaticPause()
{
	// (un)set auto pause state
	bool AutomaticPauseEnabled = AutomaticPauseManager::isActive(AutomaticPauseManager::getInstance());
	AutomaticPauseEnabled = !AutomaticPauseEnabled;

	AutomaticPauseManager::setActive(AutomaticPauseManager::getInstance(), AutomaticPauseEnabled);

	// write state to sram
	SaveDataManager::setAutomaticPauseStatus(ProgressManager::getInstance(), AutomaticPauseEnabled);

	// update visual representation
	OptionsScreenState::updateAutomaticPauseCheckBox(this);
}

void OptionsScreenState::processUserInputModeShowOptions(UserInput userInput)
{
	if((K_A | K_B | K_STA | K_SEL) & userInput.pressedKey)
	{
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
		this->option = (this->option > 0) ? this->option - 1 : kOptionScreenOptionBrightness;
		OptionsScreenState::updateCursorPosition(this);
	}
	else if((K_LD | K_RD) & userInput.pressedKey)
	{
		this->option = (this->option < kOptionScreenOptionBrightness) ? this->option + 1 : 0;
		OptionsScreenState::updateCursorPosition(this);
	}
	else if((K_LR | K_RR | K_LL | K_RL) & userInput.pressedKey)
	{
		switch(this->option)
		{
			case kOptionScreenOptionLanguage:
				OptionsScreenState::switchLanguage(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;

			case kOptionScreenOptionAutomaticPause:
				OptionsScreenState::toggleAutomaticPause(this);
				break;

			case kOptionScreenOptionBrightness:
				OptionsScreenState_switchBrightness(this, (K_LR & userInput.pressedKey) || (K_RR & userInput.pressedKey));
				break;
		}
	}
}

void OptionsScreenState::processUserInput(UserInput userInput)
{
	switch(this->mode)
	{
		case kOptionsScreenModeShowOptions:
		{
			OptionsScreenState::processUserInputModeShowOptions(this, userInput);
			break;
		}
	}
}

// handle event
void OptionsScreenState::switchState()
{
	Base::switchState(this);

	Game::changeState(Game::getInstance(), GameState::safeCast(TitleScreenState::getInstance()));
}
