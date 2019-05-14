#include "Pool.h"
#include "GameStateMachine.h"
#include "GameObject.h"
#include "GOFactory.h"

GameObject * nap_Pool::spawn()
{
	GameObject* tmp = GOFactory::GetGOPrefab(entityName); //creates a new GO based on the prefab
	//tmp->id().name_ = entityName + std::to_string(pool.size()); //no so names are the expected
	pool.push_back(tmp);
	GameStateMachine::getSingleton()->currentState()->addGameObject(tmp);
	return tmp;
}

nap_Pool::nap_Pool(std::string entity): entityName(entity)
{
	//getting prefab
	//original = GOFactory::GetGOPrefab(entity);
}

nap_Pool::~nap_Pool()
{
	/*for (int i = 0; i < pool.size(); i++) {
		if (pool[i] != nullptr) {
			delete(pool[i]);
			pool[i] = nullptr;
		}
	}*/   //deleted by the gamestate
}

void nap_Pool::resetPool() {
		for (auto & go : pool) go->setActive(false);
}

GameObject * nap_Pool::getItem()
{
	//Returns the 1º aviable item, if there's none it creates a new one
	int i = 0;
	while (i < pool.size())
	{
		if (!pool[i]->isActive()) {
			pool[i]->setActive();
			return pool[i];
		}
		i++;
	}

	return spawn();
}

void nap_Pool::init()
{
	//Creates a default value of items to populate the pool from the start
	for (int i = 0; i < defaultValue; i++) {
		spawn()->setActive(false, true);
	}
}
