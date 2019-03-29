#include "FloatingEnemyComponent.h"
#include "PhysicsComponent.h"


void FloatingEnemyComponent::WaveyMovement()
{
	rigidBody->addForce({10, 0, 0});
}

void FloatingEnemyComponent::setUp()
{
}

void FloatingEnemyComponent::lateSetUp()
{
	rigidBody = static_cast<PhysicsComponent*>(o->getComponent("basic_phy"))->getDynamicBody();
}

void FloatingEnemyComponent::update(GameObject * ent, float time)
{
	WaveyMovement();
}

bool FloatingEnemyComponent::handleEvents(GameObject * ent, const SDL_Event & evt)
{
	return false;
}
