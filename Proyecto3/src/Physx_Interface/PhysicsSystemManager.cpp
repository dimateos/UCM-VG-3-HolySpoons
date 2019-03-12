#include "PhysicsSystemManager.h"
#include <iostream> //temp debugin

//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.
//#define PVD_HOST "127.0.0.1" for the visual debugger attached

//Singleton patern setup
PhysicsSystemManager* PhysicsSystemManager::instance_ = nullptr;
PhysicsSystemManager* PhysicsSystemManager::getSingleton() {
	if (instance_ == nullptr) {
		instance_ = new PhysicsSystemManager();
	}

	return instance_;
}

//Singleton patern setdown
void PhysicsSystemManager::shutdownSingleton() {
	if (instance_ == nullptr) return;

	delete instance_;
	instance_ = nullptr;
}

// Initialize physics engine
void PhysicsSystemManager::setupInstance() {
	// Foundation
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, errorCallback_);

#if PVD // Visual debugger attached
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport *transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
#endif

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, NULL); //gPvd instead of NULL

	// Materials
	material_ = gPhysics->createMaterial(0.5f, 0.5f, 0.6f); //not color -> friction etc
	bouncyMaterial_ = gPhysics->createMaterial(0.5f, 0.5f, 1.0f);

	// Scene config
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.gravity = { 0, -9.8, 0 };
	// Collisions
	sceneDesc.filterShader = PxDefaultSimulationFilterShader; //contactReportFilterShader
	sceneDesc.simulationEventCallback = &eventReporter_;

	gScene = gPhysics->createScene(sceneDesc);

	// Add custom application code ?
	PxTransform px(PxVec3(0.0f));
	physx::PxShape* shape = gPhysics->createShape(PxSphereGeometry(3.0f), *material_);

	rigidBodyD = gPhysics->createRigidDynamic(px);
	rigidBodyD->attachShape(*shape);
	gScene->addActor(*rigidBodyD);

	rigidBodyS = gPhysics->createRigidStatic(px);
	rigidBodyS->attachShape(*shape);
	gScene->addActor(*rigidBodyS);

	//end of custom code
}

// Cleanup physics engine
void PhysicsSystemManager::shutdownInstance() {
	// Add custom application code ?
	//end custom

	// Scene
	gScene->release();
	gDispatcher->release();

	// Foundation etc
	gPhysics->release();

#if PVD // Visual debugger attached
	PxPvdTransport *transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
#endif

	gFoundation->release();
}

///////////////////////////////////////////////////////////////////////////

// Function to configure what happens in each step of physics
// t: time passed since last call in milliseconds
void PhysicsSystemManager::stepPhysics(double t) {
	gScene->simulate(t);
}

void PhysicsSystemManager::updateNodes() {
	gScene->fetchResults(true);

	// some temp debugging
	std::cout
		<< "static: " << rigidBodyS->getGlobalPose().p.y
		<< " dynamic: " << rigidBodyD->getGlobalPose().p.y
		<< std::endl << std::endl;

	return; //temp

	// retrieve array of actors that MOVED (so we just update the least possible nodes)
	PxU32 nbActiveActors;
	PxActor** activeActors = gScene->getActiveActors(nbActiveActors);

	// update each render object with the new transform
	for (PxU32 i = 0; i < nbActiveActors; ++i) {
		//gotta put here the ogre node as renderObject and userData on the creation of the rigidBodies
		//MyRenderObject* renderObject = static_cast<MyRenderObject*>(activeActors[i]->userData);
		//renderObject->setTransform(activeActors[i]->getGlobalPose());
	}

}
