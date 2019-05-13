//Nap_Time_Studios
#ifndef UPGRADECOMPONENT_H_
#define UPGRADECOMPONENT_H_

#include "CollisionListener.h"
#include "Component.h"


class UpgradeComponent : public Component, public CollisionListener
{
public:
	inline UpgradeComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~UpgradeComponent() {};
	virtual void setUp() {};
	virtual void update(GameObject* o, double time) {
		//o->setPosition(nap_vector3(0, 5, 0));
	};

	virtual void onCollision(ID* other);
	virtual void setUpgrade(int upgrade);

protected:
	int upgrade_ = -1;
};

#endif /* UPGRADECOMPONENT_H_ */
