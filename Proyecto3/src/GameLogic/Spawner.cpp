#include "GameStateMachine.h"
#include "MessageSystem.h"
#include "GOFactory.h"
#include "Spawner.h"
#include "Pool.h"

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
	radius_ = cfg_["radius"];
	t.start(cfg_["timer"]);

	pol->init();
}

nap_vector3 Spawner::smartPositioning(nap_vector3 pos)
{
	//target position
	nap_vector3 trgPos = GameStateMachine::getSingleton()->currentState()->getPlayer()->getPosition();
	//We get the direction
	nap_vector3 vec = { trgPos.x_ - pos.x_, 0, trgPos.z_ - pos.z_ };

	//We muiltiply the vector by the radius to ensure the item spawns outside the bounding box EJ:5
	vec = vec.normalize() * radius_;

	//We add the vector to our position and return the resulting position
	return pos + vec;
}

void Spawner::update(GameObject * o, double time)
{
	if (!o->isKilled()) { //Shoulld not be required
		if (t.update(time)) {
			t.start();	//Timer reset

			GameObject* tmp = pol->getItem(); //gets object from pool, spawns it
			if (!smart) tmp->setPosition(o->getPosition());	//Basic Spawn point
			else tmp->setPosition(smartPositioning(owner_->getPosition()));		//Smart spawn point
		}
	}
}

void Spawner::receive(Message * msg) {
	if (msg->id_ == RESET_PULL) {
		pol->resetPool();
		t.start();
	}
}

REGISTER_TYPE(Spawner);