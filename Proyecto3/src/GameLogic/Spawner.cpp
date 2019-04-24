#include "Spawner.h"
#include "Pool.h"
#include "GOFactory.h"
#include "MessageSystem.h"

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

	pol->init();
}

void Spawner::update(GameObject * o, double time)
{
	lastActiveT += time;
	if(lastActiveT > timer){ //simple timer
		lastActiveT = 0;//gets object from pool, spawns it
		GameObject* tmp = pol->getItem();
		tmp->setPosition(o->getPosition());
		tmp->setActive();
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(HP_RESET), tmp);
	}
}

REGISTER_TYPE(Spawner);
