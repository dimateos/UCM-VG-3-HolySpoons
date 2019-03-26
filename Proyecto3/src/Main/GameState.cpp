#include "GameState.h"
#include "LogSystem.h"

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

	for (GameObject* o : gameObjects_) {
		if (o != nullptr) o->setUp();
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
void GameState::update(float time) {

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