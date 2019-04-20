#include "BulletHit.h"
#include "LogSystem.h"


void BulletHit::setUp() { 
	setCollisionListener(owner_);
}

void BulletHit::onCollision(ID * other) {
	if(other->group_ != owner_->id().group_)
		owner_->setActive(false);
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHit);
