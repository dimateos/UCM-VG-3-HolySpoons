#include "SinusoidalMovement.h"
#include <math.h>
#include <Transforms.h>
#include "GOFactory.h"

void SinusoidalMovementComponent::configActive()
{
	timer = 0;
}

void SinusoidalMovementComponent::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	amplitude = cfg_["Amplitude"];
	speed = cfg_["speed"];

	if (FIND(cfg_, "moveX"))  moveX = this->cfg_["moveX"];
	if (FIND(cfg_, "moveY"))  moveY = this->cfg_["moveY"];
	if (FIND(cfg_, "moveZ"))  moveZ = this->cfg_["moveZ"];
	
}

void SinusoidalMovementComponent::late_update(GameObject * ent, double time)
{
	nap_vector3 ownerPos = owner_->getPosition();
	//timer -> sin mov
	timer += time;

	//sinusoidal movement
	if (moveX)
		ownerPos.x_ += sin(timer*speed)*amplitude;
	if (moveY)
		ownerPos.y_ += sin(timer*speed)*amplitude;
	if (moveZ)
	    ownerPos.z_ += sin(timer*speed)*amplitude;

	owner_->setPosition(ownerPos);
}

REGISTER_TYPE(SinusoidalMovementComponent);

