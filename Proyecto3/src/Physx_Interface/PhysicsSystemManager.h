//Nap_Time_Studios
#pragma once

#include <ctype.h>
#include <PxPhysicsAPI.h>
#include "callbacks.hpp"

using namespace physx;

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);

private:
	void onCollision(physx::PxActor* actor1, physx::PxActor* actor2);

	// Foundation and Scene
	PxDefaultAllocator gAllocator;
	PxFoundation *gFoundation	= NULL;
	PxPhysics *gPhysics			= NULL;
	PxScene *gScene				= NULL;
	PxDefaultCpuDispatcher *gDispatcher = NULL;
	//PxPvd *gPvd				= NULL; //visual debugger

	// Materials
	PxMaterial *gMaterial		= NULL;
	PxMaterial *bouncyMaterial	= NULL;

	// Collisions
	ContactReportCallback gContactReportCallback;
	PxDefaultErrorCallback gErrorCallback;

	///////////////////////////////////////////////////////////////////////////

	static PhysicsSystemManager* instance_; //singleton pattern
	void setupInstance();
	void shutdownInstance();

	//private constructor
	PhysicsSystemManager() { setupInstance(); };
	~PhysicsSystemManager() { shutdownInstance(); };
};
