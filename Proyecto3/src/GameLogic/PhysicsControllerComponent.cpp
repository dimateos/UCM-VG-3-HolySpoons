#include "PhysicsControllerComponent.h"
#include "LogSystem.h"

#include <PhysicsSystemManager.h>
#include <Transforms.h>

#include <PxRigidDynamic.h>
//#include <characterkinematic\PxController.h>
#include <characterkinematic\PxCapsuleController.h>

//some global cfg?
#define BaseRadius 2.0f
#define BaseHeight 3.0f
#define BaseDens 1.0		//atm mass not defined
#define BaseImass 14.0		//inverse
#define BaseLinDamp 0.05	//def 0.0 and 1 max

#define BaseContactOffset 0.01f
#define BaseStepOffset 0.05f
#define BaseMinDist 0.001

#define BaseSlopeLimit 0.0f
#define BaseInvisibleWallsHeight 0.0f
#define BaseMaxJumpHeight 0.0f

void PhysicsControllerComponent::setUp() {
	if (isInited()) return;
	setInited();

	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();

	PxCapsuleControllerDesc desc;
	//desc.type = PxControllerShapeType::eCAPSULE;

	//pos
	PxExtendedVec3 pos(.0, .0, .0);
	pos += owner_->getPosition().px();
	desc.position = pos;
	desc.upDirection = vY.px();

	//shape
	desc.radius = FINDnRETURN(cfg_, "radius", float, BaseRadius);
	desc.height = FINDnRETURN(cfg_, "height", float, BaseHeight);
	std::string mat = FINDnRETURN_s(cfg_, "material", BaseMat);
	desc.material = physicsManager->getMaterial(mat);
	desc.density = FINDnRETURN(cfg_, "dens", float, BaseDens); //no update mass?
	i_mass = FINDnRETURN(cfg_, "i_mass", float, BaseImass);
	damping_ = FINDnRETURN(cfg_, "damping", float, BaseLinDamp);

	//main behaviour
	desc.contactOffset = FINDnRETURN(cfg_, "contactOffset", float, BaseContactOffset);
	desc.stepOffset = FINDnRETURN(cfg_, "stepOffset", float, BaseStepOffset);
	minDist = FINDnRETURN(cfg_, "minDist", float, BaseMinDist);

	//more specific behaviour
	desc.slopeLimit = FINDnRETURN(cfg_, "slopeLimit", float, BaseSlopeLimit);
	desc.invisibleWallHeight = FINDnRETURN(cfg_, "invisibleWallsHeight", float, BaseInvisibleWallsHeight);
	desc.maxJumpHeight = FINDnRETURN(cfg_, "maxJumpHeight", float, BaseMaxJumpHeight);

	//unused atm
	//desc.reportCallback = this;
	//desc.behaviorCallback = this;

	//create the controller
	controller_comp = physicsManager->createController(&desc);

	//custom gravity
	g_ = FINDnRETURN(cfg_, "g", nap_vector3, pxG);

	updateUserData();
	configActive();
	//LogSystem::Log("scale: ", owner_->getScale().json(), LogSystem::DEV);
}

void PhysicsControllerComponent::setDown() {
	//release the controller (which releases the actor, shape etc)
	controller_comp->release();
}

void PhysicsControllerComponent::configActive() {
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !active_);
}

void PhysicsControllerComponent::updateUserData() {
	controller_comp->setPosition(owner_->getPosition().pxEx());

	if (ud_ != nullptr) delete ud_;
	ud_ = new nap_userData(owner_->getTransPtr(), owner_->getCollisionListeners(), owner_->idPtr());

	//controller_->setUserData(ud_); //unused atm
	getActor()->userData = ud_;
}

PxRigidActor * PhysicsControllerComponent::getActor() {
	return controller_comp->getActor();
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsControllerComponent::update(GameObject * o, double time) {
	//update a
	nap_vector3 frame_f = f_ * i_mass * time;
	a_ = a_ + frame_f;

	//update v
	v_ = v_ + (a_ + g_) * time;
	v_ = v_ * powf(1-damping_, time);

	//update pos
	nap_vector3 prevPos = napVEC3(controller_comp->getPosition());
	nap_vector3 newPos = prevPos + v_ * time;

	//calculate disp
	nap_vector3 disp = newPos - prevPos;

	//MOVE THE CONTROLLER
	controller_comp->move(disp.px(), minDist, time, NULL, NULL);

	//integrate real v
	nap_vector3 finalPos = napVEC3(controller_comp->getPosition());
	v_ = (finalPos - prevPos) / time;

	//clear force
	a_ = a_ - frame_f;
	f_ = vO;

	//LogSystem::Log("prevPos: ", prevPos.json(), LogSystem::DEV);
	//LogSystem::Log("newPos: ", newPos.json(), LogSystem::DEV);
	//LogSystem::Log("finalPos: ", finalPos.json(), LogSystem::DEV);
	//LogSystem::Log("finalV: ", v_.json(), LogSystem::DEV);
}

void PhysicsControllerComponent::late_update(GameObject * o, double time) {
	//the controller ignores changes on orientation
	if (o->getUpToDate(upToDate::pos, upToDate::PHYS)) return;

	controller_comp->setPosition(o->getPosition().pxEx());
	o->setUpToDate(upToDate::pos, upToDate::PHYS);
}

#include "GOFactory.h"
REGISTER_TYPE(PhysicsControllerComponent);