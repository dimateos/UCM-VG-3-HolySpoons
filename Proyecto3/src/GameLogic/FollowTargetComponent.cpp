#include "FollowTargetComponent.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>
#include "GOFactory.h"

#include <Transforms.h>

void FollowTargetComponent::follow(float t)
{
	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = {dest->p_.x_ - ownerPos.x_, dest->p_.y_ - ownerPos.y_, dest->p_.z_ - ownerPos.z_}; //simple vector calc dest - src (we dont want to follow the y coord)
	dir = dir.normalize();

	//timer
	timer += t;

	//move towards dest
	ownerPos.x_ += dir.x_*t*speed * followX;
	ownerPos.y_ += dir.y_*t*speed * followY;
	ownerPos.z_ += dir.z_*t*speed * followZ;

	owner_->setPosition(ownerPos);
}

void FollowTargetComponent::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	speed = cfg_["HorizontalSpeed"];
	if (FIND(cfg_, "FollowX")) followX = this->cfg_["FollowX"];	
	if (FIND(cfg_, "FollowY")) followY = this->cfg_["FollowY"];
	if (FIND(cfg_, "FollowZ")) followZ = this->cfg_["FollowZ"];

	string target = this->cfg_["Target"];
	GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject(target);

	if (o != nullptr) {
		dest = o->getTransPtr();
	}
	else {
		LogSystem::Log("GameObject " + target + " does not exist in the current state", LogSystem::GAME,  {__FILE__, __LINE__} );
	}
}

void FollowTargetComponent::update(GameObject* ent, double time)
{
	follow(time);
}

REGISTER_TYPE(FollowTargetComponent);
