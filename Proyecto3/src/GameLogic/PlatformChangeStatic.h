//Nap_Time_Studios
#ifndef PLATCHANGESTATIC_H_
#define PLATCHANGESTATIC_H_

class PhysicsComponent; //fowarded

#include "CollisionListener.h"
#include "Component.h"
//component to make platforms fall / become dynamic on player contact
class PlatformChangeStatic : public Component, public CollisionListener
{
public:
	inline PlatformChangeStatic(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~PlatformChangeStatic() {};
	virtual void setUp();
	virtual void lateSetUp();

	virtual void onCollision(ID* other);

protected:
	virtual void configActive();

	nap_vector3 fallSpeed_;
	PhysicsComponent * phy_;
	std::string playerName_;
};

#endif /* PLATCHANGESTATIC_H_ */
