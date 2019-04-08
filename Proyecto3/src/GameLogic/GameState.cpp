#include "GameState.h"
#include "LogSystem.h"
#include <SoundManager.h>

GameState::GameState() : Initiable(), gameObjects_() {}
GameState::GameState(std::list<GameObject*> gObjects) : Initiable(), gameObjects_(gObjects) {}

GameState::~GameState() {
	setDown();
}

void GameState::setUp() {
	if (isInited()) return;
	setInited();

	// musiquita que venia de ejemplo jeje. Si quereis ver el 3D modificad la z y lo oireis por la izq o der
	// siempre hacer el setListenerTransform y como parametro un puntero a la pos&rot del player (o de lo que vaya a escuchar)
	// la primera vez que se llame al getSingleton
	SoundManager::getSingleton()->setListenerTransform(player_->getTransPtr());
	//setting the monkey as emitter to test pushing it around
	SoundManager::getSingleton()->play3DSound("ophelia.mp3", &getGameObject("monkey_boombox")->getTransPtr()->p_, true, false);

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

//#include "GameStateMachine.h" //avoid fowarded rewriting
//so states do not need this include and is cleaner

//void GameState::sendToGSM(Message * msg) {
//	send(msg, game_->getGSM());
//}

///////////////////////////////////////////////////////////////////////////////

//sends the event too all the entities
//no controlled propagation, cause it had limitations
bool GameState::handleEvents(const SDL_Event evt) {
	bool handled = false;

	auto it = gameObjects_.begin();
	while (!handled && it != gameObjects_.end()) {
		if (!(*it)->isActive()) continue;
		handled = (*it)->handleEvents(evt);
		it++;
	}

	return handled;
}

//iterates all the Entities and calls their updates
void GameState::update(double time) {
	//first remove msg/collision killed objects + last frame updates
	killDeadObjects();

	//LogSystem::Log("state update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive())o->update(time);
	}

	//LogSystem::Log("state late update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive() && !o->isKilled())o->late_update(time);
	}
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