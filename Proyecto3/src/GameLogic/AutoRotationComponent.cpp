#include "AutoRotationComponent.h"
#include "GameStateMachine.h"

void AutoRotationComponent::setUp()
{
	if (isInited()) return;
	setInited();
}

void AutoRotationComponent::update(GameObject * ent, double time)
{
	//set rotation
	dest = GameStateMachine::getSingleton()->currentState()->getPlayer()->getPosition();
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(DIRECTOR, dest - ent->getPosition()));
}

AutoRotationComponent::~AutoRotationComponent()
{
}
