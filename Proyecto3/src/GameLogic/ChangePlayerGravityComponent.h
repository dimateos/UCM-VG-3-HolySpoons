#ifndef CHANGEPLAYERGRAVITYCOMPONENT_H_
#define CHANGEPLAYERGRAVITYCOMPONENT_H_

#include "Component.h"

class PhysicsControllerComponent;

class ChangePlayerGravityComponent: public Component
{
private:
	PhysicsControllerComponent* controller = nullptr;
	int active_message, inactive_message;
	nap_vector3 initial_gravity, custom_gravity;

public:
	virtual ~ChangePlayerGravityComponent() {};
	ChangePlayerGravityComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void lateSetUp();

	virtual void receive(Message* msg);
};

#endif //CHANGEPLAYERGRAVITYCOMPONENT_H_