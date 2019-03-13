#include "PhysicsComponent.h"
#include <iostream> //temp debug

void PhysicsComponent::update(GameObject * ent, float time) {
	// some temp debugging

	std::cout
		<< "static: " << rigidBodyS->getGlobalPose().p.y
		<< " dynamic: " << rigidBodyD->getGlobalPose().p.y
		<< std::endl << std::endl;
}

void PhysicsComponent::setUp() {
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();

	PxTransform trans (PxVec3(0.0f, 10.0f, 0.0f), PxQuat());
	rigidBodyD = physicsManager->createDynamicBody(PxBoxGeometry(PxVec3(1.0f)), trans);
	//rigidBodyD->userData;
	rigidBodyS = physicsManager->createStaticBody(PxBoxGeometry(10.0f, 1.0f, 10.0f), PxTransform (PxVec3(0.0f)));
	//rigidBodyS->userData;
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	rigidBodyD->release();
	rigidBodyS->release();
}
