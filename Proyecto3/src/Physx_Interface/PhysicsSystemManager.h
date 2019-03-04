//Nap_Time_Studios
#pragma once

#include <ctype.h>
#include <PxPhysicsAPI.h>

#include "EventReporter.h"
using namespace physx;

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);

private:
	// Foundation and Scene
	PxDefaultAllocator gAllocator;
	PxFoundation *gFoundation	= NULL;
	PxPhysics *gPhysics			= NULL;
	PxScene *gScene				= NULL;
	PxDefaultCpuDispatcher *gDispatcher = NULL;
	//PxPvd *gPvd				= NULL; //visual debugger

	// Materials
	PxMaterial *material_		= NULL;
	PxMaterial *bouncyMaterial_	= NULL;

	// Collisions and events
	EventReporter eventReporter_;
	PxDefaultErrorCallback errorCallback_; //unused

	///////////////////////////////////////////////////////////////////////////

	static PhysicsSystemManager* instance_; //singleton pattern
	void setupInstance();
	void shutdownInstance();

	//private constructor
	PhysicsSystemManager() { setupInstance(); };
	~PhysicsSystemManager() { shutdownInstance(); };
};
