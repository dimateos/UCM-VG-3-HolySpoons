#include "BulletHittable.h"
#include "LogSystem.h"

#include "GameStateMachine.h"
#include "MessageSystem.h"

//at some point move to json
std::map<string, int> BulletHittable::bulletDamages_ = { {"bBullets0", 12}, {"pBullets0", 30} };

void BulletHittable::setUp() {
	maxHitPoints_ = hitPoints_ = FINDnRETURN(cfg_, "hp", int, 0);
	valuePoints_ = FINDnRETURN(cfg_, "points", int, 50);
	enemy = FINDnRETURN(cfg_, "enemy", bool, true);
	deactivate = FINDnRETURN(cfg_, "deactivate", bool, true);
}

void BulletHittable::configActive() {
	hitPoints_ = maxHitPoints_;
}

void BulletHittable::onCollision(ID * other) {
	if (!owner_->isActive() || !isActive())return;

	//find the damage value of the impacted bullet
	if (bulletDamages_.find(other->group_) != bulletDamages_.end())
		if (hitPoints_ > 0) {
			hitPoints_ -= bulletDamages_.at(other->group_);
			MessageSystem::getSingleton()->sendMessageGameObject(
				&Message(ENEMY_DAMAGE), GameStateMachine::getSingleton()->currentState()->getGM());
		}

	//check death
	if (hitPoints_ <= 0) {
		if(deactivate)owner_->setActive(false); // if bulletHittable has to deactivate its owner, it does
		else MessageSystem::getSingleton()->sendMessageGameObject(&Message(DEACTIVATE_OBJECT), owner_); // else, it sends a message to its owner

		MessageSystem::getSingleton()->sendMessageGameObject(
			&Msg_BULLET_HIT(valuePoints_, enemy), GameStateMachine::getSingleton()->currentState()->getGM());
	}
}

void BulletHittable::receive(Message * msg) {
	if (msg->id_ == RESET_HP) configActive();
}


#include "GOFactory.h"
REGISTER_TYPE(BulletHittable);
