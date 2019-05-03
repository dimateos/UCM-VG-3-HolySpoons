//Nap_Time_Studios
#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "Component.h"
#include <Timer.h>
class nap_Pool;

class Spawner : public Component
{
private:
	napTimer t;

	bool smart = false;
	float radius_;

	nap_Pool* pol = nullptr;
	list<Component*> compList;

	nap_vector3 smartPositioning(nap_vector3 pos);

public:
	inline Spawner(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~Spawner();

	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual void receive(Message* msg);
};

#endif /* SPAWNER_H_ */

