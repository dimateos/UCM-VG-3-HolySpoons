#include "BulletHittable.h"
#include "LogSystem.h"

void BulletHittable::setUp() {}

void BulletHittable::onCollision(ID * other) {
	if (other->group_ == "bullets") {
		owner_->setKilled();
	}
}
