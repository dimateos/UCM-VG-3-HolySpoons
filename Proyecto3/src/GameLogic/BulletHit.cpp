#include "BulletHit.h"
#include "LogSystem.h"

void BulletHit::setUp() {}

void BulletHit::onCollision(ID * other) {
	owner_->setKilled();
}
