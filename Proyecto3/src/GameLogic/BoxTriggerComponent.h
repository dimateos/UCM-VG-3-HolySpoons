#ifndef BOXTRIGGERCOMPONENT_H_
#define BOXTRIGGERCOMPONENT_H_

#include "Component.h"

class GameObject;

class BoxTriggerComponent: public Component
{
private:
	GameObject* target = nullptr;
	float axis;
	bool x, y, z, inside;

public:
	virtual ~BoxTriggerComponent() {};
	BoxTriggerComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void lateSetUp();

	virtual void update(GameObject* o, double time);
};

#endif //BOXTRIGGERCOMPONENT_H_