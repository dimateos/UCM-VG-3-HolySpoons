#ifndef SINUSOIDALMOVEMENTCOMPONENT_H_
#define SINUSOIDALMOVEMENTCOMPONENT_H_

#include "Component.h"

class SinusoidalMovementComponent : public Component
{
private:

	//logic
	float timer = 0, rotation = 0;
	nap_transform* dest;

	//cfg (json)
	bool moveX = false, moveY = false, moveZ = false;
	float amplitude = 1, speed = 1;


	virtual void configActive();

public:
	virtual ~SinusoidalMovementComponent() {};
	SinusoidalMovementComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual void late_update(GameObject* ent, double time);
};

#endif //SINUSOIDALMOVEMENTCOMPONENT_H_