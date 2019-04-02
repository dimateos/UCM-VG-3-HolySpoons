#include "AutoRotationComponent.h"

void AutoRotationComponent::setUp()
{
}

void AutoRotationComponent::update(GameObject * ent, float time)
{
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(DIRECTOR, dest - ent->getPosition()));
}

AutoRotationComponent::~AutoRotationComponent()
{
}
