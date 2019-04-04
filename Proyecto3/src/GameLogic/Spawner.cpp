#include "Spawner.h"
#include "Pool.h"


Spawner::Spawner(nap_json const & cfg): Component(cfg)
{
	//json parameters
	pol = new nap_Pool(cfg["itemString"]);
	pol->setDefault(cfg["default"]);
	timer = cfg["timer"];
	active = cfg["active"];
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
	if (active) {
		if(lastActiveT < time){ //simple timer
			lastActiveT = time + timer;
			//gets object from pool, spawns it
			GameObject* tmp = pol->getItem();
			tmp->setPosition(o->getPosition());
		}
	}
}
