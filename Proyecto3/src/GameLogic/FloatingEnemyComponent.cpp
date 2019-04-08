#include "FloatingEnemyComponent.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>

void FloatingEnemyComponent::WaveyMovement(float t)
{
	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = {dest->p_.x_ - ownerPos.x_, 0, dest->p_.z_ - ownerPos.z_}; //simple vector calc dest - src (we dont want to follow the y coord)
	dir = dir.normalize();

	//timer -> sin mov
	timer += t;

	//sinusoidal movement
	ownerPos.y_ = sin(timer*vertSpeed)*amplitude + initY;

	//move towards dest
	ownerPos.x_ += dir.x_*t*horiSpeed;
	ownerPos.z_ += dir.z_*t*horiSpeed;

	//circular movement
	//float angle = horiSpeed * timer;
	//initPos.x_ = cos(angle) * 10;
	//initPos.z_ = sin(angle) * 10;

	owner_->setPosition(ownerPos);
}

void FloatingEnemyComponent::setUp()
{
	LogSystem::Log("HOWDY PARTNER!", LogSystem::SPAWNER);
	if (isInited()) return;
	setInited();

	//json parameters
	amplitude = cfg_["Amplitude"];
	horiSpeed = cfg_["HorizontalSpeed"];
	vertSpeed = cfg_["VerticalSpeed"];
	initY = owner_->getPosition().y_;
	dest = GameStateMachine::getSingleton()->currentState()->getPlayer()->getTransPtr(); //should be a pointer...
}

void FloatingEnemyComponent::update(GameObject* ent, double time)
{
	WaveyMovement(time);
}
