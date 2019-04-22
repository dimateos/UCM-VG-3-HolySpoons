#include "PhysicsSystemManager.h"
#include "LogSystem.h"

//required
#include "Transforms.h"
#include "CollisionListener.h"

//fowarded
#include "EventReporter.h"
//#include <ctype.h>
#include <PxPhysicsAPI.h>
#include <characterkinematic\PxControllerManager.h>

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
	gAllocator = new PxDefaultAllocator();
	errorCallback_ = new PxDefaultErrorCallback();
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, *gAllocator, *errorCallback_);

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
	sceneDesc.gravity = pxG.px();

	// Materials
	//atm here but should be on global cfg file etc
	mats_["BASE"] = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
	mats_["BOUNCY"] = gPhysics->createMaterial(0.5f, 0.5f, 1.0f);
	mats_["PLAYER"] = gPhysics->createMaterial(0.0f, 0.0f, 0.0f);

	// Collisions
	sceneDesc.filterShader = EventReporter::contactReportFilterShader; //PxDefaultSimulationFilterShader
	eventReporter_ = new EventReporter();
	sceneDesc.simulationEventCallback = eventReporter_;

	// Finally scneen
	gScene = gPhysics->createScene(sceneDesc);
	gScene->setFlag(PxSceneFlag::eENABLE_ACTIVE_ACTORS, true);

	gControllerManager = PxCreateControllerManager(*gScene);

	// Add custom application code ?
	//end of custom code
}

// Cleanup physics engine
void PhysicsSystemManager::shutdownInstance() {
	// Add custom application code ?
	//end custom

	// release all rigidBodies!

	// Scene
	gControllerManager->release();
	gScene->release();
	delete eventReporter_;
	gDispatcher->release();

	// Foundation etc
	gPhysics->release();

#if PVD // Visual debugger attached
	PxPvdTransport *transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
#endif

	gFoundation->release();
	delete errorCallback_;
	delete gAllocator;
}

///////////////////////////////////////////////////////////////////////////

// Function to configure what happens in each step of physics
// t: time passed since last call in milliseconds
void PhysicsSystemManager::stepPhysics(double t) {
	if (paused) return;
	gScene->simulate(t);
	//LogSystem::Log("Simulating phys", LogSystem::PHYS);
}

//updates moved objects transforms
void PhysicsSystemManager::updateNodes() {
	if (paused) return;
	gScene->fetchResults(true);
	//LogSystem::Log("UpdateNodes phys", LogSystem::PHYS);

	// retrieve array of actors that MOVED (so we just update the least possible nodes)
	PxU32 nbActiveActors;
	PxActor** activeActors = gScene->getActiveActors(nbActiveActors);

	// update each render object with the new transform
	for (PxU32 i = 0; i < nbActiveActors; ++i) {
		auto px_trans = static_cast<PxRigidActor *>(activeActors[i])->getGlobalPose();
		nap_userData* nap_UD = static_cast<nap_userData*>(activeActors[i]->userData);

		//if (nap_UD->id_->name_ == "player") LogSystem::Log();

		//update gameobject transform
		nap_UD->trans_->p_ = napVEC3(px_trans.p);
		nap_UD->trans_->q_ = napQUAT(px_trans.q);
		nap_UD->trans_->upToDate_rend = false; //render flag not upToDate
	}
}

///////////////////////////////////////////////////////////////////////////

PxScene * PhysicsSystemManager::getScene() {
	return gScene;
}

PxMaterial* PhysicsSystemManager::getMaterial(std::string mat) {
	return mats_[mat];
}

PxShape* PhysicsSystemManager::createShape(PxGeometry *geo, std::string mat) {
	return gPhysics->createShape(*geo, *mats_[mat]);
}

PxRigidDynamic * PhysicsSystemManager::createDynamicBody(PxShape* shape, PxTransform* trans) {
	PxRigidDynamic * body = gPhysics->createRigidDynamic(trans == nullptr ? PxTransform() : *trans);
	body->attachShape(*shape);
	body->setLinearDamping(0.1f);
	body->setLinearVelocity(PxVec3(0.0f));
	PxRigidBodyExt::updateMassAndInertia(*body, 1.0f);
	gScene->addActor(*body);

	return body;
}

PxRigidStatic * PhysicsSystemManager::createStaticBody(PxShape* shape, PxTransform* trans) {
	PxRigidStatic * body = gPhysics->createRigidStatic(trans == nullptr ? PxTransform() : *trans);
	body->attachShape(*shape);
	gScene->addActor(*body);

	return body;
}

PxController * PhysicsSystemManager::createController(PxControllerDesc * desc) {
	return gControllerManager->createController(*desc);;
}
