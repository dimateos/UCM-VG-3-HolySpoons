#include "BoxTriggerComponent.h"
#include "GameStateMachine.h"
#include "MessageSystem.h"

bool BoxTriggerComponent::is_inside()
{
	nap_vector3 position = target->getPosition();

	return (position.x_ >= this->x && position.x_ < this->x + this->w 
		&& position.y_ <= this->y && position.y_ > this->y + this->h 
			&& position.z_ >= this->z && position.z_ < this->z + this->d);
}

void BoxTriggerComponent::setUp()
{
	if (isInited()) return;
	setInited();
}

void BoxTriggerComponent::lateSetUp() {
	target = GameStateMachine::getSingleton()->currentState()->getGameObject(this->cfg_["target"]);

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
			MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Enter(owner_), target);
			already_inside = true;
		}
	}
	else if(already_inside){
		already_inside = false;
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_Trigger_Exit(owner_), target);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BoxTriggerComponent);

