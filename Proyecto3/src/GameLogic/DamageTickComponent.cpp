#include "DamageTickComponent.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"


void DamageTickComponent::setUp() {
	if (isInited()) return;
	setInited();

	timer.setDuration(cfg_["timer"]);
	timer.stop();
	dmg = cfg_["dmg"];
	enterId = cfg_["enterId"];
	exitId = cfg_["exitId"];
	target_ = GameStateMachine::getSingleton()->currentState()->getGameObject(cfg_["target"]);
}

void DamageTickComponent::receive(Message * msg)
{
	if (msg->id_ == (MessageId)enterId) {
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_HP_DAMAGE(dmg), target_);
		active_ = true;
		timer.start();
	}
	else if (msg->id_ == (MessageId)exitId) {
		active_ = false;
		timer.stop();
	}
}

void DamageTickComponent::update(GameObject * ent, double time)
{
	if (timer.update(time)) {
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_HP_DAMAGE(dmg), target_);
		timer.start();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(DamageTickComponent);
