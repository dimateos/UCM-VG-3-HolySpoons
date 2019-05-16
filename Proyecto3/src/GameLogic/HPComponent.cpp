#include "HPComponent.h"

#include "GameStateMachine.h"
#include "MessageSystem.h"

void HPComponent::setUp()
{
	if (isInited()) return;
	setInited();

	InitHP = HP = cfg_["hp"];
	incrementHP = FINDnRETURN(cfg_, "incrHp", float, 10);

	// if you want the gameObject to emit a sound when hit
	// default value "" -> not playing any sound when hit
	soundEmitter = FINDnRETURN_s(cfg_, "soundEmitter", "");
}

void HPComponent::receive(Message * msg)
{
	if (msg->id_ == HP_DAMAGE) {
		subHP(static_cast<Msg_HP_DAMAGE*>(msg)->damage_);
		if(soundEmitter != "")
			MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message(PLAY_SOUND), owner_, soundEmitter);

		//tell GM to check
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(CHECK_HP), GameStateMachine::getSingleton()->currentState()->getGM());
	}
	else if (msg->id_ == RESET_HP) {
		resetHP();
	}
	else if (msg->id_ == ADD_HP) {
		InitHP += incrementHP;
		resetHP();
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(CHECK_HP), GameStateMachine::getSingleton()->currentState()->getGM());
	}
}

#include "GOFactory.h"
REGISTER_TYPE(HPComponent);