#include "PhysicsComponent.h"

void PhysicsComponent::setUp(nap_json const & cfg) {
	//get the correct built shape
	PxGeometry *geo = getShape(cfg["shape"]);

	//different body for dynamic or static
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	dynamic = cfg["dynamic"];
	if (dynamic) {
		rigidBodyD = physicsManager->createDynamicBody(geo, PxTransform());
		//mass and stuff
	}
	else { //static
		rigidBodyS = physicsManager->createStaticBody(geo, PxTransform());
	}

	//only after ogre node update... but need the ogre object soo...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	getActor()->release();
}

void PhysicsComponent::setUserData(nap_transform* trans) {
	getActor()->userData = trans;
}
nap_transform * PhysicsComponent::getUserData() {
	return static_cast<nap_transform*> (getActor()->userData);
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsComponent::receive(Message * msg) {
	switch (msg->id_) {
	case PX_USERPTR:
		auto trans = static_cast<Msg_PX_userPtr*> (msg)->trans_;
		setUserData(trans);
		//actualy update the transform and avoid phsx limits + set raw upToDate true
		getActor()->setGlobalPose(PxTransform(trans->p_.px(), trans->q_.px()));
		trans->upToDate_phys = true;
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////

bool PhysicsComponent::handleEvents(GameObject * o, const SDL_Event & evt) { return false; }
void PhysicsComponent::update(GameObject * o, float time) {}

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
		geo = new PxBoxGeometry(nap_vector3(shape).px() / 2); //half extents
	}

	return geo;
}