#include "DestructibleSpawner.h"
#include "Pool.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"

void DestructibleSpawner::setUp() {
	Spawner::setUp();
	owner_->setActive(false); // it begins deactivated

	// we add the spawner to the game manager's destructible spawners list
	MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_SPAWNER(owner_), 
		GameStateMachine::getSingleton()->currentState()->getGM());
}

void DestructibleSpawner::receive(Message * msg) {
	Spawner::receive(msg);

	// when you reactive this spawner, you add himself to the GM (player will have to kill it)
	if (msg->id_ == ACTIVE_SPAWNER) {
		t.resetTimeLeft();
		owner_->setActive(true);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(DestructibleSpawner);
