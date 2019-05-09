#include "BoxTriggerComponent.h"
#include "GameStateMachine.h"
#include "MessageSystem.h"

void BoxTriggerComponent::setUp()
{
	if (isInited()) return;
	setInited();
}

void BoxTriggerComponent::lateSetUp() {
	target = GameStateMachine::getSingleton()->currentState()->getGameObject(this->cfg_["target"]);

	if (FIND(this->cfg_, "x")) { x = true;  axis = owner_->getPosition().x_; }
	if (FIND(this->cfg_, "y")) { y = true;  axis = owner_->getPosition().y_; }
	if (FIND(this->cfg_, "x")) { z = true;  axis = owner_->getPosition().z_; }

	inside = false;
}

void BoxTriggerComponent::update(GameObject * o, double time)
{
	if (target->getPosition().y_ <= axis) {
		if (!inside) {
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Enter(owner_), target);
			inside = true;
		}
	}
	else if(inside){
		inside = false;
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Exit(owner_), target);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BoxTriggerComponent);

