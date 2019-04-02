#include "AutoRotationComponent.h"

void AutoRotationComponent::setUp()
{
	if (isInited()) return;
	setInited();
}

void AutoRotationComponent::update(GameObject * ent, double time)
{
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(DIRECTOR, dest - ent->getPosition()));
}

AutoRotationComponent::~AutoRotationComponent()
{
}
