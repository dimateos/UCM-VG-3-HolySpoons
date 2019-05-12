#include "BoxTriggerComponent.h"
#include "GameStateMachine.h"
#include "MessageSystem.h"

bool BoxTriggerComponent::is_inside()
{
	return (position->p_.x_ >= this->x && position->p_.x_ < this->x + this->w 
		&& position->p_.y_ <= this->y && position->p_.y_ > this->y + this->h 
			&& position->p_.z_ >= this->z && position->p_.z_ < this->z + this->d);
}

void BoxTriggerComponent::setUp()
{
	if (isInited()) return;
	setInited();
}

void BoxTriggerComponent::lateSetUp() {
	target = GameStateMachine::getSingleton()->currentState()->getGameObject(this->cfg_["target"]);
	position = target->getTransPtr();

	x = this->cfg_["x"];
	y = this->cfg_["y"];
	z = this->cfg_["z"];
	w = this->cfg_["w"];
	h = this->cfg_["h"];
	d = this->cfg_["d"];

	already_inside = false;
}

void BoxTriggerComponent::update(GameObject * o, double time)
{
	if (is_inside()) {
		if (!already_inside) {
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Enter(owner_), target); //send message to the object entering the trigger
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Enter(owner_), owner_); //send message to the owner of the trigger
			already_inside = true;
		}
	}
	else if(already_inside){
		already_inside = false;
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Exit(owner_), target); //send message to the object exiting the trigger
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Exit(owner_), owner_); //send message to the owner of the trigger
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BoxTriggerComponent);

