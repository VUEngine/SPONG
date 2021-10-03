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

#include <Game.h>
#include <ProgressManager.h>
#include <AutomaticPauseManager.h>
#include <AutomaticPauseScreenState.h>
#include <PrecautionScreenState.h>


//---------------------------------------------------------------------------------------------------------
//											GAME'S MAIN LOOP
//---------------------------------------------------------------------------------------------------------

int main(void)
{
	// initialize plugins
	ProgressManager::restoreSettings(ProgressManager::getInstance());
	AutomaticPauseManager::setAutomaticPauseState(AutomaticPauseManager::getInstance(), GameState::safeCast(AutomaticPauseScreenState::getInstance()));
	AutomaticPauseManager::setActive(AutomaticPauseManager::getInstance(), true);

	// start the game
	Game::start(Game::getInstance(), GameState::safeCast(PrecautionScreenState::getInstance()));

	// end program
	return true;
}
