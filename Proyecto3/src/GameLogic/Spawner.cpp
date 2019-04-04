#include "Spawner.h"
#include "Pool.h"


Spawner::Spawner(nap_json const & cfg): Component(cfg)
{
	pol = new nap_Pool(cfg["itemString"]);
	pol->setDefault(cfg["default"]);
	timer = cfg["timer"];
	active = cfg["active"];
}

Spawner::~Spawner()
{
	delete pol;
}

void Spawner::setUp()
{
	pol->init();
}

void Spawner::update(GameObject * o, double time)
{
	if (active) {
		if(lastActiveT < time){
			lastActiveT = time + timer;
			GameObject* tmp = pol->getItem();
			tmp->setPosition(o->getPosition());
		}
	}
}
