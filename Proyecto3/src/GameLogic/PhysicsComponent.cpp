#include "PhysicsComponent.h"
#include "LogSystem.h"

void PhysicsComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the correct built shape
	PxGeometry *geo = getShape(cfg_["shape"]);

	//different body for dynamic or static
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	if (cfg_["dynamic"]) {
		rigidBodyD_ = physicsManager->createDynamicBody(geo, PxTransform());
		//mass and stuff
	}
	else { //static
		rigidBodyS_ = physicsManager->createStaticBody(geo, PxTransform());
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
	getActor()->release();
}

void PhysicsComponent::updateUserData() {
	getActor()->userData = owner_->getTransPtr();
	getActor()->setGlobalPose(PxTransform(owner_->getPosition().px(), owner_->getOrientation().px()));
}


///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::receive(Message * msg) {
	switch (msg->id_) {
	}
}

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