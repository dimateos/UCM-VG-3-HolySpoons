#include "HPComponent.h"
#include "GOFactory.h"

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
	}
}

REGISTER_TYPE(HPComponent);