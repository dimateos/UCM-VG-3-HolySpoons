//Nap_Time_Studios
#ifndef BOMBARDIERBEHAVIOR_H_
#define BOMBARDIERBEHAVIOR_H_

#include "Component.h"
#include "Pool.h"
#include <Timer.h>



class BombardierBehavior :
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
	nap_vector3 orig{0,0,0};

	//cfg (json)
	int charge = 10, currentCharge = 10;
	float speed = 0.1, highY = 0, lowY = 0;
	state status = advance;

public:

	inline BombardierBehavior() {};

	inline BombardierBehavior(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~BombardierBehavior() {};

	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

#endif /* BOMBARDIERBEHAVIOR_H_ */
