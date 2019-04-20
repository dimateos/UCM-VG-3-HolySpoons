#include "BulletHittable.h"
#include "LogSystem.h"

void BulletHittable::setUp() {
	setCollisionListener(owner_);
}

void BulletHittable::onCollision(ID * other) {
	//LogSystem::Log("hitted " + owner_->id().name_);
	if (other->group_ == "bullets") {
		owner_->setActive(false);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHittable);
