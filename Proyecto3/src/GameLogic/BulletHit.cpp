#include "BulletHit.h"
#include "LogSystem.h"
#include "GOFactory.h"

void BulletHit::setUp() { 
	setCollisionListener(owner_);
}

void BulletHit::onCollision(ID * other) {
	owner_->setActive(false);
}

REGISTER_TYPE(BulletHit);
