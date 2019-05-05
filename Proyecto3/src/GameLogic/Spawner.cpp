#include "Spawner.h"
#include "Pool.h"

#include <SDL_events.h>
#include "GameStateMachine.h"
#include "MessageSystem.h"

#include "LogSystem.h"

Spawner::~Spawner() {
	if (pol != nullptr) {
		delete pol;
		pol = nullptr;
	}
}

void Spawner::setUp() {
	if (isInited()) return;
	setInited();

	//json parameters
	pol = new nap_Pool(cfg_["itemString"]);
	pol->setDefault(cfg_["default"]);

	smart = cfg_["smart"];
	if (smart) radius_ = cfg_["radius"];

	t.start(cfg_["timer"]);
	pol->init();
}

void Spawner::spawn() {
	GameObject* tmp = pol->getItem(); //gets object from pool, spawns it
	if (!smart) tmp->setPosition(owner_->getPosition());	//Basic Spawn point
	else tmp->setPosition(smartPositioning(owner_->getPosition()));		//Smart spawn point
}

nap_vector3 Spawner::smartPositioning(nap_vector3 pos) {
	//target position
	nap_vector3 trgPos = GameStateMachine::getSingleton()->currentState()->getPlayer()->getPosition();
	//We get the direction
	nap_vector3 vec = { trgPos.x_ - pos.x_, 0, trgPos.z_ - pos.z_ };

	//We muiltiply the vector by the radius to ensure the item spawns outside the bounding box EJ:5
	vec = vec.normalize() * radius_;

	//We add the vector to our position and return the resulting position
	return pos + vec;
}

void Spawner::update(GameObject * o, double time) {
	if (t.update(time)) {
		t.start();	//Timer reset

		// numToSpawn == -1 -> infinite spawning
		if (numToSpawn == -1) {
			spawn();
			// we add 1 to the GM foreach enemy spwaned
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_ENEMY(1), 
				GameStateMachine::getSingleton()->currentState()->getGM());
		}
		// numToSpawn > 0 -> limit of enemies to spawn
		else if (numToSpawn > 0) {
			spawn();
			numToSpawn--;
		}
	}
}

bool Spawner::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

#if _DEBUG //HAXS
	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == SDLK_o) {
			LogSystem::Log("Manually spawned go", LogSystem::SPAWNER);
			spawn();
			handled = true;
		}
	}
#endif

	return handled;
}

void Spawner::receive(Message * msg) {
	if (msg->id_ == RESET_PULL) {
		pol->resetPool();
		t.start();
	}
	// when you reset a limited spawner, you reset its numToSpawn
	else if (msg->id_ == RESET_SPAWNER) {
		t.resetTimeLeft();
		numToSpawn = static_cast<Msg_RESET_SPAWNER*>(msg)->num_;
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_ENEMY(numToSpawn),
			GameStateMachine::getSingleton()->currentState()->getGM());
	}
	// when you reactive a ilimited spawner (destructible) you add himself to the GM (you will have to destroy it)
	else if (msg->id_ == ACTIVE_SPAWNER) {
		t.resetTimeLeft();
		owner_->setActive(true);
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_ENEMY(1),
			GameStateMachine::getSingleton()->currentState()->getGM());
	}
}

#include "GOFactory.h"
REGISTER_TYPE(Spawner);