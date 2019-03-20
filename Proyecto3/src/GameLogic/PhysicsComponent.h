//Nap_Time_Studios
#pragma once

#include "PhysicsSystemManager.h"
#include <OgreSceneNode.h>

#include "Component.h"
//tmp proof of concept Render+Phyiscs
class PhysicsComponent : public Component
{
public:
	PhysicsComponent() : Component() { setUp(); };
	virtual ~PhysicsComponent() { setDown(); };

	virtual void update(GameObject* ent, float time);
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);

	virtual void loadParameters(std::vector<string>parameters);

protected:
	virtual void setUp();
	virtual void setDown();

	PxRigidStatic* rigidBodyS = nullptr;
	PxRigidDynamic* rigidBodyD = nullptr;

	Ogre::SceneNode *nodeS = nullptr, *nodeD = nullptr;
};