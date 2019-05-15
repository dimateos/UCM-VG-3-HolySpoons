//Nap_Time_Studios
#ifndef BULLETHITTRIGGER_H_
#define BULLETHITTRIGGER_H_

#include <Timer.h>
class PhysicsComponent; //fowarded

#include "CollisionListener.h"
#include "Component.h"
//component to active / deactive bullets
//tries to set the bullet initially as a trigger so the player doesnt collide with it and then change it to normal
//performance friendly compared to filtering collisions
class BulletHitTrigger : public Component, public CollisionListener
{
public:
	inline BulletHitTrigger(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~BulletHitTrigger() {};
	virtual void setUp();
	virtual void lateSetUp();

	virtual void configActive();

	virtual void update(GameObject* o, double time);
	virtual void onCollision(ID* other);

protected:
	void configIniInv();
	bool iniTrigger;
	float time, time_inv;
	PhysicsComponent * phy;

	napTimer t;
};

#endif /* BULLETHITTRIGGER_H_ */
