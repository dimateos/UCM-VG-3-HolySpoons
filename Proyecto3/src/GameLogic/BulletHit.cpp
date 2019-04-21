#include "BulletHit.h"
#include "LogSystem.h"

void BulletHit::setUp() {}

void BulletHit::onCollision(ID * other) {
	if (other->group_ != owner_->id().group_)
		owner_->setActive(false);
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHit);
