//Nap_Time_Studios
#ifndef BULLETHITABLE_H_
#define BULLETHITABLE_H_

#include "CollisionListener.h"

#include "Component.h"
class BulletHittable : public Component, public CollisionListener
{
public:
	inline BulletHittable() {}
	inline BulletHittable(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~BulletHittable() {};
	virtual void setUp();

	virtual void onCollision(ID* other);
};

#endif /* BULLETHITABLE_H_ */
