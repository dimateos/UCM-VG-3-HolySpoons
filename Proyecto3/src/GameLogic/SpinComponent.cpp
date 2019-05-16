#include "SpinComponent.h"
#include <LogSystem.h>
#include "RenderComponent.h"
#include <OgreSceneNode.h>

void SpinComponent::setUp()
{
	if (isInited()) return;
	setInited();

	if (FIND(this->cfg_, "Roll")) { roll_ = this->cfg_["Roll"]; }
	if (FIND(this->cfg_, "Pitch")) { pitch_ = this->cfg_["Pitch"]; }
	if (FIND(this->cfg_, "Yaw")) { yaw_ = this->cfg_["Yaw"]; }
}

void SpinComponent::lateSetUp()
{
	string component_name = this->cfg_["RenderComponent"];
	RenderComponent* rc = static_cast<RenderComponent*>(this->getOwner()->getComponent(component_name));

	if (rc != nullptr) {
		node = rc->getSceneNode();
	}
	else {
		LogSystem::Log("Compponent " + component_name + "could not be found in the GameObject", LogSystem::GAME, { __FILE__, __LINE__ });
	}
}

SpinComponent::~SpinComponent()
{
}

void SpinComponent::late_update(GameObject * ent, double time)
{
	rotate(ent, time);
}

void SpinComponent::rotate(GameObject * ent, double time)
{
	//set rotation
	node->pitch(Ogre::Radian(pitch_ * time)); //x axis
	node->yaw(Ogre::Radian(yaw_ * time)); //y axis
	node->roll(Ogre::Radian(roll_ * time)); //z axis
}


#include "GOFactory.h"
REGISTER_TYPE(SpinComponent);
