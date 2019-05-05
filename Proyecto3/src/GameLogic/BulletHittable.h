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
	static std::map<string, int> bulletDamages_;

	inline BulletHittable(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~BulletHittable() {};
	virtual void setUp();

	virtual void onCollision(ID* other);
	virtual void receive(Message* msg);

protected:
	virtual void configActive();

	int hitPoints_ = 0, maxHitPoints_ = 0, valuePoints_;
	bool enemy;
};

#endif /* BULLETHITABLE_H_ */
