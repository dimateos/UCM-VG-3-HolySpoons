#include "GameStateMachine.h"
#include "MessageSystem.h"
#include "GOFactory.h"
#include "Spawner.h"
#include "Pool.h"

nap_vector3 Spawner::smartPositioning(GameObject * o)
{
	//target position
	nap_transform* trgPos = GameStateMachine::getSingleton()->currentState()->getPlayer()->getTransPtr();
	//We get the direction
	nap_vector3 vec = { trgPos->p_.x_ - o->getTransPtr()->p_.x_, 0, trgPos->p_.z_ - o->getTransPtr()->p_.z_ };
	//We normalizer the vector to length 1
	vec = vec.normalize();

	//We muiltiply the vector by the radius to ensure the item spawns outside the bounding box EJ:5
	vec *= 5;
	//We add the vector to our position and return the resulting position
	return o->getTransPtr()->p_ + vec;
}

Spawner::~Spawner()
{
	if (pol != nullptr) {
		delete pol;
		pol = nullptr;
	}
}

void Spawner::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	pol = new nap_Pool(cfg_["itemString"]);
	pol->setDefault(cfg_["default"]);
	timer = cfg_["timer"];
	//We have to read the smart boolean
	smart = cfg_["smart"];

	pol->init();
}

void Spawner::update(GameObject * o, double time)
{
	if (!o->isKilled()) { //Shoulld not be required
		lastActiveT += time;
		if (lastActiveT > timer) {					//simple timer
			lastActiveT = 0;						//Timer reset
			GameObject* tmp = pol->getItem();		//gets object from pool, spawns it
			if (!smart)
				tmp->setPosition(o->getPosition());	//Basic Spawn point
			else tmp->setPosition(smartPositioning(o));//Smart spawn point
			tmp->setActive();
			MessageSystem::getSingleton()->sendMessageGameObject(&Message(HP_RESET), tmp);
		}
	}
}

void Spawner::receive(Message * msg) {
	if (msg->id_ == RESET_PULL) {
		pol->resetPool();
		lastActiveT = 0;
	}
}

REGISTER_TYPE(Spawner);
