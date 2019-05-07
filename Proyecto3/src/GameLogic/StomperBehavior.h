//Nap_Time_Studios
#ifndef StomperBehavior_H_
#define StomperBehavior_H_

#include "Component.h"
#include "Pool.h"
#include <Timer.h>



class StomperBehavior :
	public Component
{
protected:
	enum e_state { move, jump, rocket, shield };

	void moveBehavior(float time);
	void jumpBehavior(float time);
	void rocketBehavior(float time);
	void shieldBehavior(float time);
	void shoot();
	virtual void configActive();

	//logic
	napTimer t;
	nap_Pool* pRocket,* pBullet,* pSpawn;

	nap_transform* dest;
	vector<nap_vector3> positions, directions;

	//cfg (json)
	float speed = 0.1, actionDelay = 2, shotSpeed = 50, lowY = 2, highY = 10, baseY = lowY;
	int i = 0, maxBullets = 3, maxJumps = 2, lastAttack = 3;
	e_state status = move;
	bool up = true;

public:

	inline StomperBehavior(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~StomperBehavior() {};

	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

#endif /* StomperBehavior_H_ */
