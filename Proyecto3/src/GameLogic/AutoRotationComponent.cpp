#include "AutoRotationComponent.h"
#include "GameStateMachine.h"

void AutoRotationComponent::setUp()
{
	if (isInited()) return;
	setInited();

	dest = GameStateMachine::getSingleton()->currentState()->getPlayer()->getTransPtr();
}

void AutoRotationComponent::update(GameObject * ent, double time)
{
	//set rotation
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(DIRECTOR, dest->p_ - ent->getPosition()));
}

AutoRotationComponent::~AutoRotationComponent()
{
}
