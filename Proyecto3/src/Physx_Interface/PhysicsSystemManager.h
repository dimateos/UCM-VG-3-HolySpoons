//Nap_Time_Studios
#pragma once

#include <ctype.h>
#include <PxPhysicsAPI.h>
#define PVD 0 //visual debbuger
using namespace physx;

#include "EventReporter.h"

// Materials
#define BaseMat "BASE"
#define BaseDens 1			//atm mass not defined
#define BaseLinDamp 0.05	//def 0.0 and 1 max
#define BaseAngDamp 0.05	//def 0.05 and 1 max
#define BaseMaxAngV 100		//fast spinning objects should raise this (def 100 in px4.0)
#include <map>

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);
	void updateNodes();

	inline void setGravity(PxVec3 v) { gScene->setGravity(v); }

	PxRigidDynamic* createDynamicBody(PxGeometry *geo, PxTransform const &trans, std::string mat = BaseMat);
	PxRigidStatic* createStaticBody(PxGeometry *geo, PxTransform const &trans, std::string mat = BaseMat);

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

	// Collisions and events
	std::map<std::string, PxMaterial*> mats_; //different materials
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