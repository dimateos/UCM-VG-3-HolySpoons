#include "FloatingEnemyComponent.h"
#include "PhysicsComponent.h" //SHOULD NOT BE HERE
#include <iostream>
#include <math.h>

void FloatingEnemyComponent::WaveyMovement(float t)
{
	timer += t;
	initPos.y_ = sin(timer*vertSpeed)*amplitude;
	initPos.x_ += horiSpeed;
	
	o->setPosition(initPos);
	cout <<"X: "<< o->getOrientation().x_ << "Y: " << o->getOrientation().y_ << "Z: " << o->getOrientation().z_ << "W: " << o->getOrientation().w_ << endl;
}

void FloatingEnemyComponent::setUp()
{
	amplitude = cfg_["Amplitude"];
	horiSpeed = cfg_["HorizontalSpeed"];
	vertSpeed = cfg_["VerticalSpeed"];
}

void FloatingEnemyComponent::lateSetUp()
{
	//THIS SHOULD BE DONE VIA THE JSON
	rigidBody = static_cast<PhysicsComponent*>(o->getComponent("basic_phy"))->getDynamicBody();
	rigidBody->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	initPos = o->getPosition();

}

void FloatingEnemyComponent::update(GameObject * ent, float time)
{
	WaveyMovement(time);
}

bool FloatingEnemyComponent::handleEvents(GameObject * ent, const SDL_Event & evt)
{
	return false;
}
