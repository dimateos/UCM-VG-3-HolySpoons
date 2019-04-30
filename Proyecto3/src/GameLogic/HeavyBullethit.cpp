#include "HeavyBulletHit.h"
#include "BulletHittable.h"
#include "LogSystem.h"


void HeavyBulletHit::onCollision(ID * other) {
	if (BulletHittable::bulletDamages_.find(other->group_) != BulletHittable::bulletDamages_.end())
		return;
	if (other->group_ != "map") {
		owner_->setActive(false);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(HeavyBulletHit);
