#include "ChangePlayerGravityComponent.h"
#include "PhysicsControllerComponent.h"
#include "GameStateMachine.h"
#include "Messages.h"

void ChangePlayerGravityComponent::setUp() {
	if (isInited()) return;
	setInited();

	active_message = this->cfg_["Active_Message"];
	inactive_message = this->cfg_["Inactive_Message"];
	custom_gravity = { this->cfg_["Custom_Gravity"]["x"], this->cfg_["Custom_Gravity"]["y"], this->cfg_["Custom_Gravity"]["z"] };
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
	}
	else if (msg->id_ == (MessageId)inactive_message) {
		controller->setG(initial_gravity);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(ChangePlayerGravityComponent);
