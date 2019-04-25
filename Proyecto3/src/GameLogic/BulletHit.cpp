#include "BulletHit.h"
#include "LogSystem.h"

void BulletHit::setUp() { t.setDuration(cfg_["time"]); }

void BulletHit::update(GameObject * o, double time)
{
	if (t.update(time))
		owner_->setActive(false);
}

void BulletHit::onCollision(ID * other) {
	if (other->group_ != owner_->id().group_)
		owner_->setActive(false);
}

void BulletHit::configActive()
{
	t.start();
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHit);
