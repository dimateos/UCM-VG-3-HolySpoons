#include "BulletHit.h"
#include "LogSystem.h"

void BulletHit::setUp() {
	setCollisionListener(owner_);
}

void BulletHit::onCollision(ID * other) {
	//LogSystem::Log("hitted " + owner_->id().name_);
	owner_->setActive(false);
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHit);
