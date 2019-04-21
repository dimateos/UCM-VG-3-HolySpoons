#include "BulletHittable.h"
#include "LogSystem.h"

//at some point defined in a config file?
std::map<string, int> BulletHittable::bulletDamage = { {"bBullets0", 10}, {"pBullets0", 30} };

void BulletHittable::setUp() {
	maxHitPoints = hitPoints = FINDnRETURN(cfg_, "hp", int, 0);
}

void BulletHittable::receive(Message * msg)
{
	if (msg->id_ == HP_RESET)
		hitPoints = maxHitPoints;
}

void BulletHittable::onCollision(ID * other) {
	//find the damage value of the impacted bullet
	if (bulletDamage.find(other->group_) != bulletDamage.end())
		if (owner_->isActive() && hitPoints > 0)
			hitPoints -= bulletDamage.at(other->group_);

	//check death
	if (hitPoints <= 0)
		owner_->setActive(false);
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHittable);
