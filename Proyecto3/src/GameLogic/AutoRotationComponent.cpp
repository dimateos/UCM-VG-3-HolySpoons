#include "AutoRotationComponent.h"

void AutoRotationComponent::setUp()
{
}

void AutoRotationComponent::update(GameObject * ent, double time)
{
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(ent->getPosition(), dest));
}

AutoRotationComponent::~AutoRotationComponent()
{
}
