#include "FloatingEnemyComponent.h"
#include "PhysicsComponent.h" //SHOULD NOT BE HERE
#include <iostream>
#include <math.h>

void FloatingEnemyComponent::WaveyMovement(float t)
{
	nap_vector3 dir = {destPos.x_ - owner_->getPosition().x_, 0, destPos.y_ - owner_->getPosition().z_}; //simple vector calc dest - src (we dont want to follow the y coord)
	dir = dir.normalize();

	//timer -> sin mov
	timer += t;

	//sinusoidal movement
	initPos.y_ = sin(timer*vertSpeed)*amplitude + initY;

	//move towards dest
	initPos.x_ += dir.x_*t*horiSpeed;
	initPos.z_ += dir.z_*t*horiSpeed;

	//circular movement
	//float angle = horiSpeed * timer;
	//initPos.x_ = cos(angle) * 10;
	//initPos.z_ = sin(angle) * 10;

	owner_->setPosition(initPos);
}

void FloatingEnemyComponent::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	amplitude = cfg_["Amplitude"];
	horiSpeed = cfg_["HorizontalSpeed"];
	vertSpeed = cfg_["VerticalSpeed"];
	initPos = owner_->getPosition();
	initY = initPos.y_;
}

void FloatingEnemyComponent::update(GameObject* ent, double time)
{
	WaveyMovement(time);
}
