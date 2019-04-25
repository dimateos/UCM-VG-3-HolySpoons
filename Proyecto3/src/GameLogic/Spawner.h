//Nap_Time_Studios
#ifndef SPAWNER_H_
#define SPAWNER_H_

#include "Component.h"
class nap_Pool;

class Spawner : public Component
{
private:
	double timer;
	double lastActiveT=0;

	bool smart = false;

	nap_Pool* pol = nullptr;
	list<Component*> compList;

	nap_vector3 smartPositioning(GameObject* o);

public:
	inline Spawner() {};
	inline Spawner(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~Spawner();

	virtual void setUp();

	//sets and gets
	inline void setTimer(double t) { timer = t; };

	virtual void update(GameObject* o, double time);
};

#endif /* SPAWNER_H_ */

