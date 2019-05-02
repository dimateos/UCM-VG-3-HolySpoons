#include "HPComponent.h"

#include "Messages.h"

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
		//reenviar to GM en vez del GM todo el rato en el update?
	}
}

#include "GOFactory.h"
REGISTER_TYPE(HPComponent);