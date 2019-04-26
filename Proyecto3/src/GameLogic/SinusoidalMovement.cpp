#include "SinusoidalMovement.h"
#include <math.h>
#include <Transforms.h>
#include "GOFactory.h"

void SinusoidalMovementComponent::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	amplitude = cfg_["Amplitude"];
	vertSpeed = cfg_["VerticalSpeed"];

	if (FIND(cfg_, "InitY")) { initY = this->cfg_["InitY"]; }
	else { initY = owner_->getPosition().y_; }
}

void SinusoidalMovementComponent::late_update(GameObject * ent, double time)
{
	nap_vector3 ownerPos = owner_->getPosition();
	//timer -> sin mov
	timer += time;

	//sinusoidal movement
	ownerPos.y_ = sin(timer*vertSpeed)*amplitude + initY;

	owner_->setPosition(ownerPos);
}

REGISTER_TYPE(SinusoidalMovementComponent);

