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

	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();

	//get the correct built shape
	PxGeometry *geo = getGeo(cfg_["shape"]);
	std::string mat = FINDnRETURN_s(cfg_, "material", BaseMat);
	shape_ = physicsManager->createShape(geo, mat);

	//different body for dynamic or static
	if (FINDnRETURN(cfg_, "dynamic", bool, true)) {
		rigidBodyD_ = physicsManager->createDynamicBody(shape_);

		//mass and dampings
		PxRigidBodyExt::updateMassAndInertia(*rigidBodyD_, FINDnRETURN(cfg_, "mass", float, BaseDens));
		rigidBodyD_->setLinearDamping(FINDnRETURN(cfg_, "linDamp", float, BaseLinDamp));
		rigidBodyD_->setAngularDamping(FINDnRETURN(cfg_, "angDamp", float, BaseAngDamp));
		rigidBodyD_->setMaxAngularVelocity(FINDnRETURN(cfg_, "maxAngV", float, BaseMaxAngV));
	}
	else { //static
		rigidBodyS_ = physicsManager->createStaticBody(shape_);
	}

	//more custom stuff
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, FINDnRETURN(cfg_, "noGravity", bool, false));
	updateOri_ = FINDnRETURN(cfg_, "updateOri", bool, true);

	//only after ogre node update... but need the ogre object soo...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);

	updateUserData();
	configActive();
}

PxRigidDynamic * PhysicsComponent::getDynamicBody() {
	return rigidBodyD_;
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	if(getActor() != nullptr) getActor()->release();
}

void PhysicsComponent::configActive() {
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !active_);
	//shape_->setFlag(PxShapeFlag::eSCENE_QUERY_SHAPE, active_);
}

void PhysicsComponent::updateUserData() {
	getActor()->setGlobalPose(PxTransform(owner_->getPosition().px(), owner_->getOrientation().px()));

	if (ud_ != nullptr) delete ud_;
	ud_ = new nap_userData(owner_->getTransPtr(), owner_->getCollisionListeners(), owner_->idPtr());

	getActor()->userData = ud_;
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::late_update(GameObject * o, double time) {
	if (updateOri_ && o->getUpToDate_trans(upToDate::PHYS)) return;
	else if (o->getUpToDate(upToDate::pos, upToDate::PHYS)) return;

	getActor()->setGlobalPose(PxTransform(o->getPosition().px(), o->getOrientation().px()));

	if (updateOri_) o->setUpToDate_trans(upToDate::PHYS);
	else o->setUpToDate(upToDate::pos, upToDate::PHYS);
}

///////////////////////////////////////////////////////////////////////////////

//const map<std::string, PxGeometryType::Enum> PhysicsComponent::geoTypes {
//	{ "BOX", PxGeometryType::eBOX },
//	{ "PLANE", PxGeometryType::ePLANE },
//	{ "SPHERE", PxGeometryType::eSPHERE },
//	{ "CAPSULE", PxGeometryType::eCAPSULE },
//};

PxGeometry* PhysicsComponent::getGeo(nap_json shape) {
	PxGeometry* geo = nullptr;

	//switch the type to construct the correct shape
	string type = shape["type"];
	if (type == "BOX") {
		geo = new PxBoxGeometry(nap_vector3(shape["scale"]).px() / 2); //half extents
	}

	return geo;
}

PxRigidActor * PhysicsComponent::getActor() {
	if (rigidBodyD_ != NULL && rigidBodyD_ != nullptr) return rigidBodyD_;
	else return rigidBodyS_;
}

#include "GOFactory.h"
REGISTER_TYPE(PhysicsComponent);