//Nap_Time_Studios
#pragma once

#include "PhysicsSystemManager.h"

#include "Component.h"
//tmp proof of concept Render+Phyiscs
class PhysicsComponent : public Component
{
public:
	PhysicsComponent() : Component() { setUp(); };
	virtual ~PhysicsComponent() { setDown(); };

	virtual void update(GameObject* ent, float time);
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) { return false; };

protected:
	// Create the physical body
	virtual void setUp();
	// Remove the physical body
	virtual void setDown();

	PxRigidStatic* rigidBodyS = nullptr;
	PxRigidDynamic* rigidBodyD = nullptr;
};