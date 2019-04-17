#include "CircularMovementComponent.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>
#include "GOFactory.h"

void CircularMovementComponent::CircularMovement(float t)
{
	nap_vector3 ownerPos = owner_->getPosition();

	timer += t;
	float angle = speed * timer;
	ownerPos.x_ = cos(angle) * rad;
	ownerPos.z_ = sin(angle) * rad;

	owner_->setPosition(ownerPos);
}

void CircularMovementComponent::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	speed = cfg_["Speed"];
	rad = cfg_["Radius"];

}

void CircularMovementComponent::update(GameObject* ent, double time)
{
	CircularMovement(time);
}

REGISTER_TYPE(CircularMovementComponent);
