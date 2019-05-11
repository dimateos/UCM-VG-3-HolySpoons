#include "DestructibleSpawner.h"
#include "Pool.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"
#include "RenderComponent.h"
#include "BulletHittable.h"

void DestructibleSpawner::setUp() {
	Spawner::setUp();
	owner_->setActive(false); // it begins deactivated

	 string phy_name = FINDnRETURN_s(cfg_, "phy_name", "bullet_hittable");
	 string rend_name = FINDnRETURN_s(cfg_, "rend_name", "basic_ren");

	phy_comp = static_cast<BulletHittable*>(owner_->getComponent(phy_name));
	rend_comp = static_cast<RenderComponent*>(owner_->getComponent(rend_name));

	// we add the spawner to the game manager's destructible spawners list
	MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_SPAWNER(owner_), 
		GameStateMachine::getSingleton()->currentState()->getGM());
}

void DestructibleSpawner::configActive() {
	if (!active_) {
		owner_->setActive(true, true);
		canSpawn = false;
		phy_comp->setActive(false);
		rend_comp->setActive(false);
	}
}

void DestructibleSpawner::receive(Message * msg) {
	Spawner::receive(msg);

	// when you reactive this spawner, you add himself to the GM (player will have to kill it)
	if (msg->id_ == ACTIVE_SPAWNER) {
		t.start();
		canSpawn = true;
		phy_comp->setActive(true);
		rend_comp->setActive(true);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(DestructibleSpawner);
