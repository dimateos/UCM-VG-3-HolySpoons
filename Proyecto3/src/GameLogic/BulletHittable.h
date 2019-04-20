//Nap_Time_Studios
#ifndef BULLETHITABLE_H_
#define BULLETHITABLE_H_

#include "CollisionListener.h"
#include <map>
#include "Component.h"




class BulletHittable : public Component, public CollisionListener
{
public:
	inline BulletHittable(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) 
	{
		maxHitPoints = hitPoints = FINDnRETURN(cfg, "hp", int, 0);
	};
	inline virtual ~BulletHittable() {};
	virtual void setUp();

	virtual void receive(Message* msg);
	virtual void onCollision(ID* other);
protected:
	static std::map<string, int> bulletDamage;
	int hitPoints = 0, maxHitPoints = 0;
};

#endif /* BULLETHITABLE_H_ */
