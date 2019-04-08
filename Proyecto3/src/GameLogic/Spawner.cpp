#include "Spawner.h"
#include "Pool.h"
#include "GameStateMachine.h"

Spawner::Spawner(nap_json const & cfg): Component(cfg)
{
	//json parameters
	pol = new nap_Pool(cfg["itemString"]);
	pol->setDefault(cfg["default"]);
	timer = cfg["timer"];
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
	pol->init();
}

void Spawner::update(GameObject * o, double time)
{
	lastActiveT += time;
	if(lastActiveT > timer){ //simple timer
		lastActiveT =0;//gets object from pool, spawns it
		GameObject* tmp = pol->getItem();
		tmp->setPosition(o->getPosition());
		GameStateMachine::getSingleton()->currentState()->addGameObject(tmp);
		tmp->setActive(true);
	}
}
