#include "FloatingEnemyComponent.h"
#include "PhysicsComponent.h" //SHOULD NOT BE HERE
#include <iostream>
#include <math.h>

void FloatingEnemyComponent::WaveyMovement(float t)
{
	timer += t;
	//initPos.y_ = sin(timer*vertSpeed)*amplitude + initY;
	//initPos.x_ += horiSpeed;
	float angle = horiSpeed * timer*100;
	initPos.x_ = cos(angle) * 10;
	initPos.z_ = sin(angle) * 10;

	owner_->setPosition(initPos);
	//cout <<"X: "<< o->getOrientation().x_ << " Y: " << o->getOrientation().y_ << " Z: " << o->getOrientation().z_ << " W: " << o->getOrientation().w_ << endl;
}

void FloatingEnemyComponent::setUp()
{
	if (isInited()) return;
	setInited();

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
