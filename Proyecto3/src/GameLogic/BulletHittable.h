//Nap_Time_Studios
#ifndef BULLETHITABLE_H_
#define BULLETHITABLE_H_

#include <map>

#include "CollisionListener.h"
#include "Component.h"
//component to set live and etc to stuff impacted by bullets
class BulletHittable : public Component, public CollisionListener
{
public:
	inline BulletHittable(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~BulletHittable() {};
	virtual void setUp();

	virtual void receive(Message* msg);
	virtual void onCollision(ID* other);

protected:
	static std::map<string, int> bulletDamage;
	int hitPoints = 0, maxHitPoints = 0;
};

#endif /* BULLETHITABLE_H_ */
