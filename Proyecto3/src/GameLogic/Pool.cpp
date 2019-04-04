#include "Pool.h"
#include "GameObject.h"
#include "GOFactory.h"

GameObject * nap_Pool::spawn()
{
	GameObject* tmp = new GameObject(original);
	//CLONE??????????
	pool.push_back(tmp);
	return tmp;
}

nap_Pool::nap_Pool(std::string entity)
{
	//OBETENER EL GO POR LO DE DIEGO
	//AÑLADIRLO A LA POOL
	original = GOFactory::GetGOPrefab(entity);
}

nap_Pool::~nap_Pool()
{
	for (int i = 0; i < pool.size(); i++) {
		delete(pool[i]);	
	}
	delete original;
}

GameObject * nap_Pool::getItem()
{
	//Returns the 1º aviable item, if there's none it creates a new one
	int i = 0;
	while (i < pool.size())
	{
		if (!pool[i]->isActive()) {
			return pool[i];
		}
		i++;
	}
	
	return spawn();
}

void nap_Pool::init()
{
	//Creates a default value of items to populate the pool from the start
	for (int i = 0; i < defaultValue-1; i++) {
		spawn();
		pool.back()->setActive(false);
	}
}
