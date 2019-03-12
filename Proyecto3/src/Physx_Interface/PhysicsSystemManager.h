//Nap_Time_Studios
#pragma once

#include <ctype.h>
#include <PxPhysicsAPI.h>
using namespace physx;

#define PVD 0
#include "EventReporter.h"

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);
	void updateNodes();

	// temp proof of working
	PxRigidStatic* rigidBodyS = nullptr;
	PxRigidDynamic* rigidBodyD = nullptr;

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