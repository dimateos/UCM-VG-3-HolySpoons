#include "BulletHit.h"
#include "LogSystem.h"

void BulletHit::setUp() {
	t.setDuration(cfg_["time"]);
	configActive();
}

void BulletHit::configActive() {
	t.start();
}

void BulletHit::update(GameObject * o, double time) {
	if (t.update(time)) {
		owner_->setActive(false);
	}
}

void BulletHit::onCollision(ID * other) {
	if (other->type_ != owner_->id().type_) {
		owner_->setActive(false);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHit);
