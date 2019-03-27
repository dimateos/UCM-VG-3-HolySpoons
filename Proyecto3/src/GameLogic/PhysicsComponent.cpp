#include "PhysicsComponent.h"
#include "LogSystem.h"

void PhysicsComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the correct built shape
	PxGeometry *geo = getShape(cfg_["shape"]);

	//different body for dynamic or static
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	dynamic = cfg_["dynamic"];
	if (dynamic) {
		rigidBodyD_ = physicsManager->createDynamicBody(geo, PxTransform());
		//mass and stuff
	}
	else { //static
		rigidBodyS_ = physicsManager->createStaticBody(geo, PxTransform());
	}

	//only after ogre node update... but need the ogre object soo...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);

	updateUserData();
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	getActor()->release();
}

void PhysicsComponent::setUserData(nap_transform* trans_) {
	user_trans_ = trans_;
	if (inited_) updateUserData();
}
nap_transform * PhysicsComponent::getUserData() {
	return user_trans_;
}
void PhysicsComponent::updateUserData() {
	getActor()->userData = user_trans_;
	getActor()->setGlobalPose(PxTransform(user_trans_->p_.px(), user_trans_->q_.px()));
}


///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::receive(Message * msg) {
	switch (msg->id_) {
	case PX_USERPTR:
		auto trans_ = static_cast<Msg_PX_userPtr*> (msg)->trans_;
		setUserData(trans_);
		//actualy update the transform and avoid phsx limits + set raw upToDate true
		getActor()->setGlobalPose(PxTransform(trans_->p_.px(), trans_->q_.px()));
		trans_->upToDate_phys = true;
		break;
	}
}

void PhysicsComponent::late_update(GameObject * o, float time) {
	if (o->getUpToDate_phys()) return;

	getActor()->setGlobalPose(PxTransform(o->getPosition().px(), o->getOrientation().px()));
	o->setUpToDate_phys();
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