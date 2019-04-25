#include "ControllerReporter.h"
#include "LogSystem.h"

#include <PxRigidStatic.h>

#include "CollisionListener.h"
void ControllerReporter::onShapeHit(const PxControllerShapeHit & hit) {
	nap_userData* controller_UD = (*ud_);
	nap_userData* other_UD = static_cast<nap_userData*>(hit.actor->userData);

	//send onCollision events
	for (CollisionListener* c : *controller_UD->collisionListeners_) {
		c->onCollision(other_UD->id_);
	}
	for (CollisionListener* c : *other_UD->collisionListeners_) {
		c->onCollision(controller_UD->id_);
	}
}
