#include "BulletHit.h"
#include "GameStateMachine.h"
#include "LogSystem.h"

void BulletHit::setUp() {
}

void BulletHit::onCollision(ID * other) {
	owner_->setKilled();
	LogSystem::Log("shit got me!!");
}
