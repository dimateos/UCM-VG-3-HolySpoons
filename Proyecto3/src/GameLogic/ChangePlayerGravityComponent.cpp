#include "ChangePlayerGravityComponent.h"
#include "PhysicsControllerComponent.h"
#include "GameStateMachine.h"
#include "Messages.h"

void ChangePlayerGravityComponent::setUp() {
	if (isInited()) return;
	setInited();

	active_message = this->cfg_["Active_Message"];
	inactive_message = this->cfg_["Inactive_Message"];
	custom_gravity = nap_vector3(cfg_["Custom_Gravity"]);
	stopBefore = FINDnRETURN(this->cfg_, "stopBefore", bool, true);
}

void ChangePlayerGravityComponent::lateSetUp()
{
	GameObject* player = GameStateMachine::getSingleton()->currentState()->getPlayer();
	if (player != nullptr) {
		controller = static_cast<PhysicsControllerComponent*>(player->getComponent("controller_phy"));
		initial_gravity = controller->getG();
	}
}

void ChangePlayerGravityComponent::receive(Message * msg)
{
	if (msg->id_ == (MessageId)active_message) {
		controller->setG(custom_gravity);

		if (stopBefore && controller->getV().y_< 0) controller->setV(vO);
	}
	else if (msg->id_ == (MessageId)inactive_message) {
		controller->setG(initial_gravity);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(ChangePlayerGravityComponent);
