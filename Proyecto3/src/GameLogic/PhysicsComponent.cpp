#include "PhysicsComponent.h"
#include "LogSystem.h"

#include <PhysicsSystemManager.h>
#include <Transforms.h>

#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <extensions\PxRigidBodyExt.h>

#define BaseDens 1.0		//atm mass not defined
#define BaseLinDamp 0.05	//def 0.0 and 1 max
#define BaseAngDamp 0.05	//def 0.05 and 1 max
#define BaseMaxAngV 100		//fast spinning objects should raise this (def 100 in px4.0)

void PhysicsComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the correct built shape
	PxGeometry *geo = getGeo(cfg_["shape"]);
	std::string mat = FINDnRETURN_s(cfg_, "material", BaseMat);
	shape_ = PhysicsSystemManager::getSingleton()->createShape(geo, mat);

	//only after ogre node update... but need the ogre object soo...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);

	//update trans and gravity
	ignoreTrans_ = FINDnRETURN(cfg_, "ignoreTrans", bool, false);
	updateOri_ = FINDnRETURN(cfg_, "updateOri", bool, true);
	noGravity_ = FINDnRETURN(cfg_, "noGravity", bool, false);

	//different body for dynamic or static
	configDynamicBody(FINDnRETURN(cfg_, "dynamic", bool, true));
}

PxRigidDynamic * PhysicsComponent::getDynamicBody() {
	return rigidBodyD_;
}

void PhysicsComponent::setShapeTrigger(bool b) {
	if (shape_ == nullptr) return;

	shape_->setFlag(b ? PxShapeFlag::eSIMULATION_SHAPE : PxShapeFlag::eTRIGGER_SHAPE, false);
	shape_->setFlag(b ? PxShapeFlag::eTRIGGER_SHAPE : PxShapeFlag::eSIMULATION_SHAPE, true);
}

void PhysicsComponent::setIgnoreTrans(bool b) {
	ignoreTrans_ = b;
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	if(getActor() != nullptr) getActor()->release();
	if (ud_ != nullptr) delete ud_;
	if (shape_ != nullptr && shape_->isReleasable()) shape_->release();
}

void PhysicsComponent::configActive() {
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !active_);
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, noGravity_);
	getActor()->setGlobalPose(PxTransform((!active_ ? baseTransPos : owner_->getPosition()).px(), owner_->getOrientation().px()));
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::updateUserData() {
	if (ud_ != nullptr) delete ud_;
	ud_ = new nap_userData(owner_->getTransPtr(), owner_->getCollisionListeners(), owner_->idPtr(), updateOri_);

	getActor()->userData = ud_;
}

void PhysicsComponent::late_update(GameObject * o, double time) {
	if (ignoreTrans_) return;
	PxTransform trans = getActor()->getGlobalPose();

	//check pos
	if (!o->getUpToDate(upToDate::pos, upToDate::PHYS)) {
		trans.p = o->getPosition().px();
		o->setUpToDate(upToDate::pos, upToDate::PHYS);
	}

	//now check the orientation if interested
	if (updateOri_ && !o->getUpToDate(upToDate::ori, upToDate::PHYS)) {
		trans.q = o->getOrientation().px();
		o->setUpToDate(upToDate::ori, upToDate::PHYS);
	}

	//set the transform
	getActor()->setGlobalPose(trans);
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::configDynamicBody(bool b) {
	if (getActor() != nullptr) getActor()->release();

	if (b) configDynamicActor();
	else rigidBodyS_ = PhysicsSystemManager::getSingleton()->createStaticBody(shape_);

	updateUserData();
	configActive();
}

void PhysicsComponent::configDynamicActor() {
	rigidBodyD_ = PhysicsSystemManager::getSingleton()->createDynamicBody(shape_);

	//mass and dampings
	PxRigidBodyExt::updateMassAndInertia(*rigidBodyD_, FINDnRETURN(cfg_, "mass", float, BaseDens));
	rigidBodyD_->setLinearDamping(FINDnRETURN(cfg_, "linDamp", float, BaseLinDamp));
	rigidBodyD_->setAngularDamping(FINDnRETURN(cfg_, "angDamp", float, BaseAngDamp));
	rigidBodyD_->setMaxAngularVelocity(FINDnRETURN(cfg_, "maxAngV", float, BaseMaxAngV));
}

PxGeometry* PhysicsComponent::getGeo(nap_json shape) {
	PxGeometry* geo = nullptr;

	//switch the type to construct the correct shape
	string type = shape["type"];
	if (type == "BOX") {
		geo = new PxBoxGeometry(nap_vector3(shape["scale"]).px() / 2); //half extents
	}
	else if (type == "SPHERE") {
		geo = new PxSphereGeometry(shape["r"]); //half extents
	}

	return geo;
}

PxRigidActor * PhysicsComponent::getActor() {
	if (rigidBodyD_ != NULL && rigidBodyD_ != nullptr) return rigidBodyD_;
	else return rigidBodyS_;
}

#include "GOFactory.h"
REGISTER_TYPE(PhysicsComponent);