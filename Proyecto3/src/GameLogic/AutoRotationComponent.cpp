#include "AutoRotationComponent.h"
#include "GameStateMachine.h"
#include "GOFactory.h"
#include <LogSystem.h>

void AutoRotationComponent::setUp()
{
	if (isInited()) return;
	setInited();

	string target = this->cfg_["Target"];
	GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject(target);

	if (o != nullptr) {
		dest = o->getTransPtr();
	}
	else {
		LogSystem::Log("GameObject " + target + " does not exist in the current state", LogSystem::GAME,  {__FILE__, __LINE__} );
	}
}

void AutoRotationComponent::update(GameObject * ent, double time)
{
	//set rotation
	ent->setOrientation(RenderSystemInterface::getSingleton()->getRotationFrom_To(DIRECTOR, dest->p_ - ent->getPosition()));
}

AutoRotationComponent::~AutoRotationComponent()
{
}

REGISTER_TYPE(AutoRotationComponent);
