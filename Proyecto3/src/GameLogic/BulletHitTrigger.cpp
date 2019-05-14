#include "BulletHitTrigger.h"
#include <GlobalConfig.h>

#include "LogSystem.h"
#include "PhysicsComponent.h"

void BulletHitTrigger::setUp() {
	time = FINDnRETURN(cfg_, "time", float, GlobalCFG::floats["bul_def_time"]);
	time_inv = FINDnRETURN(cfg_, "time_inv", float, GlobalCFG::floats["bul_def_time_inv"]);
}

void BulletHitTrigger::lateSetUp() {
	phy = static_cast<PhysicsComponent*>(owner_->getComponent("bullet_phys"));
	configActive();
}

void BulletHitTrigger::configActive() {
	if (!active_) return;

	iniTrigger = true;
	configIniInv();
}

void BulletHitTrigger::configIniInv() {
	//initial no collision mode
	if (iniTrigger) {
		phy->setShapeTrigger();
		t.start(time_inv);
		//LogSystem::Log("config trigger bul");
	}
	//normal mode (timed life)
	else {
		phy->setShapeTrigger(false);
		t.start(time);
		//LogSystem::Log("config normal bul");
	}
}

void BulletHitTrigger::update(GameObject * o, double time) {
	if (t.update(time)) {
		if (iniTrigger) {
			//no more trigger
			iniTrigger = false;
			configIniInv();
		}
		else {
			owner_->setActive(false);
			//LogSystem::Log("timer bul into inactive");
			LogSystem::Log("timer bul into inactive");
		}
	}
}

void BulletHitTrigger::onCollision(ID * other) {
	if (other->type_ != owner_->id().type_) {
		owner_->setActive(false);
		LogSystem::Log("collision bul into inactive w/" + other->name_);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BulletHitTrigger);
