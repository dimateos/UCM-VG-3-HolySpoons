#ifndef INDESTRUCTIBLE_SPAWNER_H_
#define INDESTRUCTIBLE_SPAWNER_H_

#include "Spawner.h"

class IndestructibleSpawner : public Spawner
{
private:
	int round_, numToSpawn;
	float spawnFactor, timeFactor, timeToSpawn;

	// algotithms that determinates the number of enemies to spawn
	// and the cooldown between them
	string spawnAlgorithm, timeAlgorithm;

	void upToRound();
	void upNumToSpawn();
	void upTimeToSpawn();

public:
	IndestructibleSpawner(nap_json const & cfg, GameObject* owner) :Spawner(cfg, owner) {}

	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual void receive(Message* msg);

	virtual ~IndestructibleSpawner() {}
};

#endif /* INDESTRUCTIBLE_SPAWNER_H_ */

