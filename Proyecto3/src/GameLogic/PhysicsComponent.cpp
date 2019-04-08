#include "PhysicsComponent.h"
#include "LogSystem.h"

void PhysicsComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the correct built shape
	PxGeometry *geo = getShape(cfg_["shape"]);
	std::string mat = FINDnRETURN_s(cfg_, "material", BaseMat);

	//different body for dynamic or static
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	if (cfg_["dynamic"]) {
		rigidBodyD_ = physicsManager->createDynamicBody(geo, PxTransform(), mat);

		//mass and dampings
		PxRigidBodyExt::updateMassAndInertia(*rigidBodyD_, FINDnRETURN(cfg_, "mass", float, BaseDens));
		rigidBodyD_->setLinearDamping(FINDnRETURN(cfg_, "linDamp", float, BaseLinDamp));
		rigidBodyD_->setAngularDamping(FINDnRETURN(cfg_, "angDamp", float, BaseAngDamp));
		rigidBodyD_->setMaxAngularVelocity(FINDnRETURN(cfg_, "maxAngV", float, BaseMaxAngV));
	}
	else { //static
		rigidBodyS_ = physicsManager->createStaticBody(geo, PxTransform(), mat);
	}

	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, cfg_["noGravity"]);

	//only after ogre node update... but need the ogre object soo...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);

	updateUserData();
	//LogSystem::Log("scale: ", owner_->getScale().json(), LogSystem::DEV);
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	if(getActor() != nullptr) getActor()->release();
}

void PhysicsComponent::updateUserData() {
	getActor()->setGlobalPose(PxTransform(owner_->getPosition().px(), owner_->getOrientation().px()));

	if (ud != nullptr) delete ud;
	else ud = new nap_userData(owner_->getTransPtr(), owner_->getCollisionListeners(), owner_->idPtr());

	getActor()->userData = ud;
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::late_update(GameObject * o, double time) {
	if (o->getTransUpToDate_phys()) return;

	getActor()->setGlobalPose(PxTransform(o->getPosition().px(), o->getOrientation().px()));
	o->setTransUpToDate_phys();
}

///////////////////////////////////////////////////////////////////////////////

//const map<std::string, PxGeometryType::Enum> PhysicsComponent::geoTypes {
//	{ "BOX", PxGeometryType::eBOX },
//	{ "PLANE", PxGeometryType::ePLANE },
//	{ "SPHERE", PxGeometryType::eSPHERE },
//	{ "CAPSULE", PxGeometryType::eCAPSULE },
//};

PxGeometry* PhysicsComponent::getShape(nap_json shape) {
	PxGeometry* geo = nullptr;

	//switch the type to construct the correct shape
	string type = shape["type"];
	if (type == "BOX") {
		geo = new PxBoxGeometry(nap_vector3(shape["scale"]).px() / 2); //half extents
	}

	return geo;
}