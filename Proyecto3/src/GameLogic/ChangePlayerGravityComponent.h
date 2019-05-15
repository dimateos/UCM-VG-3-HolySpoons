#ifndef CHANGEPLAYERGRAVITYCOMPONENT_H_
#define CHANGEPLAYERGRAVITYCOMPONENT_H_

class PhysicsControllerComponent; //fowarded

#include "Component.h"
//Component to change the player gravity with messages
class ChangePlayerGravityComponent: public Component
{
private:
	PhysicsControllerComponent* controller = nullptr;
	int active_message, inactive_message;
	nap_vector3 initial_gravity, custom_gravity;
	bool stopBefore;

public:
	virtual ~ChangePlayerGravityComponent() {};
	ChangePlayerGravityComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void lateSetUp();

	virtual void receive(Message* msg);
};

#endif //CHANGEPLAYERGRAVITYCOMPONENT_H_