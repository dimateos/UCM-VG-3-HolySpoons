#include "PlatformChangeStatic.h"

#include "LogSystem.h"

#include "GameStateMachine.h"
#include "PhysicsComponent.h"
#include <PxRigidDynamic.h>

void PlatformChangeStatic::setUp() {
	fallSpeed_ = nap_vector3(cfg_["fallSpeed"]);

	//name to check on collision
	playerName_ = GameStateMachine::getSingleton()->currentState()->getPlayer()->id().name_;
}

void PlatformChangeStatic::lateSetUp() {
	//to config dynamic and trans
	phy_ = static_cast<PhysicsComponent*>(owner_->getComponent("tile_phys"));
}

void PlatformChangeStatic::configActive() {
	phy_->setIgnoreTrans(false);
	phy_->configDynamicBody();
	phy_->getDynamicBody()->setLinearVelocity(fallSpeed_.px());
}

void PlatformChangeStatic::onCollision(ID * other) {
	if (!active_) return;

	if (other->name_ == playerName_) {
		//using config active to config later at start of update
		setActive(false);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(PlatformChangeStatic);
