#ifndef SINUSOIDALMOVEMENTCOMPONENT_H_
#define SINUSOIDALMOVEMENTCOMPONENT_H_

#include "Component.h"

class SinusoidalMovementComponent : public Component
{
private:

	//logic
	float initY; //initial Y, needed sin movement
	float timer = 0, rotation = 0;
	nap_transform* dest;

	//cfg (json)
	float amplitude = 1, horiSpeed = 0.01, vertSpeed = 1;

public:
	SinusoidalMovementComponent() {};
	virtual ~SinusoidalMovementComponent() {};
	SinusoidalMovementComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual void late_update(GameObject* ent, double time);
};

#endif //SINUSOIDALMOVEMENTCOMPONENT_H_