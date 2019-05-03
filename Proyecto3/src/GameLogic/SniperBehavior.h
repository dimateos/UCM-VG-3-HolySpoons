//Nap_Time_Studios
#ifndef SNIPERBEHAVIOR_H_
#define SNIPERBEHAVIOR_H_

#include "Component.h"
#include "Pool.h"
#include <Timer.h>



class SniperBehavior :
	public Component
{
protected:
	enum state { advance, attack, back };

	void advanceBehavior(float time);
	void attackBehavior(float time);
	void backBehavior(float time);
	void shoot();
	virtual void configActive();

	//logic
	napTimer t;
	nap_Pool* p;

	nap_transform* dest;
	vector<nap_vector3> positions;

	//cfg (json)
	float speed = 0.1, shotSpeed = 0.1, lowY = -8, highY = 5;
	state status = advance;
	bool wait = false;

public:

	inline SniperBehavior(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~SniperBehavior() {};

	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

#endif /* SNIPERBEHAVIOR_H_ */
