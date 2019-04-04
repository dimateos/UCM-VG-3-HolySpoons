#pragma once
#include "Component.h"
class nap_Pool;

class Spawner : public Component
{
private:
	double timer;
	double lastActiveT=0;

	bool active;
	nap_Pool* pol = nullptr;
public:
	Spawner() {};
	Spawner(nap_json const & cfg);
	virtual ~Spawner();

	void setUp();

	//sets and gets
	inline void setTimer(double t) { timer = t; };
	inline void setActive(bool t) { active = t; };
	inline bool getActive() { return active; };
	
	virtual void update(GameObject* o, double time);
};

