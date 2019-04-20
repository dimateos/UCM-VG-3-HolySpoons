#include "BulletHittable.h"
#include "LogSystem.h"

std::map<string, int> BulletHittable::bulletDamage = { {"bBullets0", 10}, {"pBullets0", 30} };


void BulletHittable::setUp() {}

void BulletHittable::receive(Message * msg)
{
	if (msg->id_ == HP_RESET)
		hitPoints = maxHitPoints;
}

void BulletHittable::onCollision(ID * other) {
	if (bulletDamage.find(other->group_) != bulletDamage.end())
		if (owner_->isActive() && hitPoints > 0)
			hitPoints -= bulletDamage.at(other->group_);
	if (hitPoints <= 0)
		owner_->setActive(false);
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHittable);
