#include "HPComponent.h"

#include "GameStateMachine.h"
#include "MessageSystem.h"

void HPComponent::setUp()
{
	if (isInited()) return;
	setInited();

	InitHP = HP = cfg_["hp"];
}

void HPComponent::receive(Message * msg)
{
	if (msg->id_ == HP_DAMAGE) {
		subHP(static_cast<Msg_HP_DAMAGE*>(msg)->damage_);
		//tell GM to check
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(CHECK_HP), GameStateMachine::getSingleton()->currentState()->getGM());
	}
	else if (msg->id_ == RESET_HP) resetHP();
}

#include "GOFactory.h"
REGISTER_TYPE(HPComponent);