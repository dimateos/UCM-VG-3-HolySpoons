#ifndef FOLLOWTARGETCOMPONENT_H_
#define FOLLOWTARGETCOMPONENT_H_

#include "Component.h"

class FollowTargetComponent :
	public Component
{
private:
	void follow(float t);

	//logic
	float timer = 0;
	nap_transform* dest;

	//cfg (json)
	float speed = 0.01;
	bool followX = true, followY = true, followZ = true;

public:
	/*
	 *Owner needed
	 */
	inline FollowTargetComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~FollowTargetComponent() {};

	/*
	 *Virtual setup, inits component
	*/
	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

#endif /* FOLLOWTARGETCOMPONENT_H_ */
