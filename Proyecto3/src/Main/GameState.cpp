#include "GameState.h"
#include "LogSystem.h"
#include <SoundManager.h>

GameState::GameState() : Initiable(), gameObjects_() {
}
GameState::GameState(std::list<GameObject*> gObjects) : Initiable(), gameObjects_(gObjects) {
}

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
	SoundManager::getSingleton()->play3DSound("ophelia.mp3", new nap_vector3(0, 0, 10), true, false);

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

void GameState::addGameObject(GameObject * o) {
	gameObjects_.push_back(o);
	if (isInited()) o->setUp();
}

void GameState::setPlayer(GameObject * player)
{
	player_ = player;
	addGameObject(player_);
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

	//LogSystem::Log("state update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive())o->update(time);
	}

	//LogSystem::Log("state late update", LogSystem::GAME);
	for (GameObject* o : gameObjects_) {
		if (o->isActive())o->late_update(time);
	}

	//killDeadObjects();
}

//requires sencond iteration to delete killed objects
//void GameState::killDeadObjects() {
//	auto it = entities_.begin();
//	while (it != entities_.end()) {
//		if ((*it)->isKilled()) {
//			delete (*it);
//			if (it == gameObjects_.begin()) //in case of fisrt one, we fckd up
//			{
//				gameObjects_.erase(it);
//				it = gameObjects_.begin(); //we fix
//			}
//			else gameObjects_.erase(it);
//		}
//		else ++it;
//	}
//}