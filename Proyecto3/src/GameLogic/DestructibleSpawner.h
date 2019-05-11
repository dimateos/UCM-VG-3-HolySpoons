#ifndef DESTRUCTIBLE_SPAWNER_H_
#define DESTRUCTIBLE_SPAWNER_H_

#include "Spawner.h"

class BulletHittable;
class RenderComponent;

class DestructibleSpawner : public Spawner
{
private:
	BulletHittable *phy_comp;
	RenderComponent *rend_comp;

	void Activate();
	void Deactivate();

public:
	DestructibleSpawner(nap_json const & cfg, GameObject* owner) :Spawner(cfg, owner) {}

	virtual void setUp();

	virtual void receive(Message* msg);

	virtual ~DestructibleSpawner() {}
};

#endif /* DESTRUCTIBLE_SPAWNER_H_ */

