#include "HeavyBulletHit.h"
#include "BulletShooter.h"
#include "LogSystem.h"


void HeavyBulletHit::onCollision(ID * other) {
	if (BulletShooter::bulletDamages.find(other->group_) != BulletShooter::bulletDamages.end())
		return;
	if (other->group_ != "map") {
		owner_->setActive(false);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(HeavyBulletHit);
