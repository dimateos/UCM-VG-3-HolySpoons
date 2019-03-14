//Nap_Time_Studios
#pragma once

#include <ctype.h>
#include <PxPhysicsAPI.h>
using namespace physx;

#define PVD 0
#include "EventReporter.h"

#define vX PxVec3(1.0f, 0.0f, 0.0f)
#define vY PxVec3(0.0f, 1.0f, 0.0f)
#define vZ PxVec3(1.0f, 0.0f, 1.0f)

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);
	void updateNodes();

	PxRigidDynamic* createDynamicBody(PxGeometry *geo, PxTransform const &trans);
	PxRigidStatic* createStaticBody(PxGeometry *geo, PxTransform const &trans);

private:
	// Foundation and Scene
	PxDefaultAllocator gAllocator;
	PxFoundation *gFoundation = NULL;
	PxPhysics *gPhysics = NULL;
	PxDefaultCpuDispatcher *gDispatcher = NULL;
	PxScene *gScene = NULL;

#if PVD //visual debugger
	PxPvd *gPvd				= NULL;
#endif

	// Materials
	PxMaterial *material_ = NULL;
	PxMaterial *bouncyMaterial_ = NULL;

	// Collisions and events
	EventReporter eventReporter_;
	PxDefaultErrorCallback errorCallback_; //unused atm

	///////////////////////////////////////////////////////////////////////////

	//singleton pattern
	static PhysicsSystemManager* instance_;
	void setupInstance();
	void shutdownInstance();

	//private constructor
	PhysicsSystemManager() { setupInstance(); };
	~PhysicsSystemManager() { shutdownInstance(); };
};