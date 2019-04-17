#include "BulletHittable.h"
#include "LogSystem.h"
#include "GOFactory.h"

void BulletHittable::setUp() {
	setCollisionListener(owner_);
}

void BulletHittable::onCollision(ID * other) {
	if (other->group_ == "bullets") {
		owner_->setActive(false);
	}
}

REGISTER_TYPE(BulletHittable);
