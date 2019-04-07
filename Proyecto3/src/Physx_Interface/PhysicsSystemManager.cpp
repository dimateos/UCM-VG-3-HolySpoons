#include "PhysicsSystemManager.h"

#include "LogSystem.h"
#include "CollisionListener.h"

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

	// Scene config
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.gravity = { 0.0f, -9.8f, 0.0f };

	// Materials
	//atm here but should be on global cfg file etc
	mats_["BASE"] = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	mats_["BOUNCY"] = gPhysics->createMaterial(0.5f, 0.5f, 1.0f);
	mats_["PLAYER"] = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);

	// Collisions
	sceneDesc.filterShader = EventReporter::contactReportFilterShader; //PxDefaultSimulationFilterShader
	sceneDesc.simulationEventCallback = &eventReporter_;

	gScene = gPhysics->createScene(sceneDesc);
	gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

	// Add custom application code ?
	//end of custom code
}

// Cleanup physics engine
void PhysicsSystemManager::shutdownInstance() {
	// Add custom application code ?
	//end custom

	// release all rigidBodies!

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

//updates moved objects transforms
void PhysicsSystemManager::updateNodes() {
	gScene->fetchResults(true);

	// retrieve array of actors that MOVED (so we just update the least possible nodes)
	PxU32 nbActiveActors;
	PxActor** activeActors = gScene->getActiveActors(nbActiveActors);

	// update each render object with the new transform
	for (PxU32 i = 0; i < nbActiveActors; ++i) {
		auto px_trans = static_cast<PxRigidActor *>(activeActors[i])->getGlobalPose();
		nap_userData* nap_UD = static_cast<nap_userData*>(activeActors[i]->userData);

		//update gameobject transform
		nap_UD->trans_->p_ = nap_vector3(VEC3(px_trans.p));
		nap_UD->trans_->q_ = nap_quat(QUAT(px_trans.q));
		nap_UD->trans_->upToDate_rend = false; //render flag not upToDate
	}
}

///////////////////////////////////////////////////////////////////////////

PxRigidDynamic * PhysicsSystemManager::createDynamicBody(PxGeometry *geo, PxTransform const &trans, std::string mat) {
	physx::PxShape* shape = gPhysics->createShape(*geo, *mats_[mat]);

	PxRigidDynamic * body = gPhysics->createRigidDynamic(trans);
	body->attachShape(*shape);
	body->setLinearDamping(0.1f);
	body->setLinearVelocity(PxVec3(0.0f));
	PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	gScene->addActor(*body);

	return body;
}

PxRigidStatic * PhysicsSystemManager::createStaticBody(PxGeometry *geo, PxTransform const &trans, std::string mat) {
	physx::PxShape* shape = gPhysics->createShape(*geo, *mats_[mat]);

	PxRigidStatic * body = gPhysics->createRigidStatic(trans);
	body->attachShape(*shape);
	gScene->addActor(*body);

	return body;
}
