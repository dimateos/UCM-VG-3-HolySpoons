#include "GameManager.h"
#include "GOFactory.h"

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

}

void GameManager::update(GameObject * o, double time) {
	// update of the HP HUD with the player HP
	// checks if the player HP is under 0 (DEAD panel)
	// or if the player has completed the round (NEXT ROUND panel)
}

bool GameManager::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	// ESC->Pause Menu, etc.

	return handled;
}

REGISTER_TYPE(GameManager);