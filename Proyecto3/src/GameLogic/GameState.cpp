#include "GameState.h"
#include "LogSystem.h"

#include "GameObject.h" //fowarded
#include <SDL_events.h>	//events

GameState::~GameState() {
	setDown();
}

void GameState::setUp() {
	if (isInited()) return;
	setInited();

	for (GameObject* o : gameObjects_) {
		if (o != nullptr) o->setUp();
	}
	for (GameObject* o : gameObjects_) {
		if (o != nullptr) o->lateSetUp();
	}
}

void GameState::setDown() {
	for (GameObject* o : gameObjects_) {
		if (o != nullptr) delete o;
	}

	gameObjects_.clear();
}

///////////////////////////////////////////////////////////////////////////////

//sends the event too all the entities
//no controlled propagation, cause it had limitations
bool GameState::handleEvents(const SDL_Event evt) {
	bool handled = false;

	auto it = gameObjects_.begin();
	while (!handled && it != gameObjects_.end()) {
		if ((*it)->isActive()) handled = (*it)->handleEvents(evt);
		if (!handled) it++;
	}

	return handled;
}

//iterates all the Entities and calls their updates
void GameState::update(double time) {
	//first remove msg/collision killed objects + last frame updates

	//LogSystem::Log("state update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive() && !o->isKilled())o->update(time);
	}

	//LogSystem::Log("state late update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive() && !o->isKilled())o->late_update(time);
	}

	//not used atm, better solution like a separate queue or buffer the deaths / events that kill
	//killDeadObjects();
}

//requires sencond iteration to delete killed objects
void GameState::killDeadObjects() {
	auto it = gameObjects_.begin();
	while (it != gameObjects_.end()) {

		if ((*it)->isKilled()) {
			delete (*it);
			it = gameObjects_.erase(it);
		}
		else ++it;
	}
}

///////////////////////////////////////////////////////////////////////////////

void GameState::addGameObject(GameObject* go) {
	gameObjects_.push_back(go);
	if (isInited()) {
		go->setUp();
		go->lateSetUp();
	}
}
void GameState::addGameObject(std::list<GameObject*> gos) {
	for (auto go : gos) {
		gameObjects_.push_back(go);
		if (isInited()) {
			go->setUp();
			go->lateSetUp();
		}
	}
}

void GameState::delGameObject(std::string name) {
	for (auto it = gameObjects_.begin(); it != gameObjects_.end(); it++) {
		if ((*it)->id().name_ == name) {
			gameObjects_.erase(it);
			break;
		}
	}
}
void GameState::delGameObject(GameObject* go) {
	auto it = std::find(gameObjects_.begin(), gameObjects_.end(), go);
	if (it != gameObjects_.end()) gameObjects_.erase(it);
}
void GameState::delGameObject(std::list<GameObject*> gos) {
	for (auto go : gos) {
		auto it = std::find(gameObjects_.begin(), gameObjects_.end(), go);
		if (it != gameObjects_.end()) gameObjects_.erase(it);
	}
}

GameObject* GameState::getGameObject(std::string name) {
	GameObject* found = nullptr;

	for (auto go : gameObjects_) {
		if (go->id().name_ == name) {
			found = go;
			break;
		}
	}
	return found;
}