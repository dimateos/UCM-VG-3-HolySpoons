#include "UpgradeComponent.h"
#include "LogSystem.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"

void UpgradeComponent::onCollision(ID * other) {
	if (other->name_ == "player") {
		owner_->setActive(false);
		MessageSystem::getSingleton()->sendMessageGameObject(
			&Msg_UPGRADE_TAKEN(upgrade_), GameStateMachine::getSingleton()->currentState()->getGM());

	}
}

void UpgradeComponent::setUpgrade(int upgrade)
{
	upgrade_ = upgrade;
}


#include "GOFactory.h"
REGISTER_TYPE(UpgradeComponent);
