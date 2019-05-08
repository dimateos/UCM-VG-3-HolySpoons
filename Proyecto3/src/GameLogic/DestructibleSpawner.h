#ifndef DESTRUCTIBLE_SPAWNER_H_
#define DESTRUCTIBLE_SPAWNER_H_

#include "Spawner.h"

class DestructibleSpawner : public Spawner
{
public:
	DestructibleSpawner(nap_json const & cfg, GameObject* owner) :Spawner(cfg, owner) {}

	virtual void setUp();

	virtual void receive(Message* msg);

	virtual ~DestructibleSpawner() {}
};

#endif /* DESTRUCTIBLE_SPAWNER_H_ */

