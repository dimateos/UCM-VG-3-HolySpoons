#include "DestructibleSpawner.h"
#include "Pool.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"
#include "RenderComponent.h"
#include "BulletHittable.h"

// We only activate some of its components
void DestructibleSpawner::Activate()
{
	t.start();
	canSpawn = true;
	owner_->setActive(true);
	phy_comp->setActive(true);
	rend_comp->setActive(true);
	// we play the portal soundeffect
	MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message(PLAY_SOUND), owner_, soundEmitter);
}

// We only deactivate some of its components
void DestructibleSpawner::Deactivate()
{
	canSpawn = false;
	phy_comp->setActive(false);
	rend_comp->setActive(false);
	// we stop the portal soundeffect
	MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message(STOP_SOUND), owner_, soundEmitter);
}

void DestructibleSpawner::setUp() {
	Spawner::setUp();

	string phy_name = FINDnRETURN_s(cfg_, "phy_name", "bullet_hittable");
	string rend_name = FINDnRETURN_s(cfg_, "rend_name", "basic_ren");

	phy_comp = static_cast<BulletHittable*>(owner_->getComponent(phy_name));
	rend_comp = static_cast<RenderComponent*>(owner_->getComponent(rend_name));

	soundEmitter = FINDnRETURN_s(cfg_, "soundEmitter", "portalSound");

	// we add the spawner to the game manager's destructible spawners list
	MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_SPAWNER(owner_), 
		GameStateMachine::getSingleton()->currentState()->getGM());
}

void DestructibleSpawner::receive(Message * msg) {
	Spawner::receive(msg);

	// when you reactive this spawner, you add himself to the GM (player will have to kill it)
	if (msg->id_ == ACTIVE_SPAWNER) Activate();
	// received when this spawner has been killed. We only deactivate some of its components
	else if (msg->id_ == DEACTIVATE_OBJECT) Deactivate();
}

#include "GOFactory.h"
REGISTER_TYPE(DestructibleSpawner);
