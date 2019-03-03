#include "GameState.h"

GameState::GameState() {}

GameState::~GameState() {}


///////////////////////////////////////////////////////////////////////////////

//#include "GameStateMachine.h" //avoid fowarded rewriting
//so states do not need this include and is cleaner

//void GameState::sendToGSM(Message * msg) {
//	send(msg, game_->getGSM());
//}

///////////////////////////////////////////////////////////////////////////////

//sends the event too all the entities
//no controlled propagation, cause it had limitations
void GameState::handleEvents(float time, const Event evt) {
	cout << endl << "\t\t state EVENTS" << endl;

	for (GameObject* e : entities_) {
		if (e->isActive())e->handleEvents(time, evt);
	}
}

//iterates all the Entities and calls their updates
void GameState::update(float time) {
	cout << endl << "\t\t state UPDATE" << endl;

	for (GameObject* e : entities_) {
		if (e->isActive())e->update(time);
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