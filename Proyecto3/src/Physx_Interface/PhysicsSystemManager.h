//Nap_Time_Studios
#ifndef PHYS_SYS_MANAGER_H_
#define PHYS_SYS_MANAGER_H_

#include "PhyscsFowarder.h"

#define PVD 0 //visual debbuger

// Tmp Materials
#define BaseMat "BASE"
#include <map>

class PhysicsSystemManager
{
public:
	// Creates / returns the singleton instance
	static PhysicsSystemManager* getSingleton();
	static void shutdownSingleton();

	void stepPhysics(double t);
	void updateNodes();

	//creating bodies
	PxScene* getScene();
	PxMaterial * getMaterial(std::string mat);
	PxShape * createShape(PxGeometry * geo, std::string mat);
	PxRigidDynamic* createDynamicBody(PxShape* shape, PxTransform* trans = nullptr);
	PxRigidStatic* createStaticBody(PxShape* shape, PxTransform* trans = nullptr);
	PxController* createController(PxControllerDesc *desc);

private:
	// Foundation and Scene
	PxDefaultAllocator* gAllocator;
	PxFoundation *gFoundation = NULL;
	PxPhysics *gPhysics = NULL;
	PxDefaultCpuDispatcher *gDispatcher = NULL;
	PxScene *gScene = NULL;
	PxControllerManager* gControllerManager = NULL;

#if PVD //visual debugger
	PxPvd *gPvd				= NULL;
#endif

	// Collisions and events
	std::map<std::string, PxMaterial*> mats_; //different materials
	EventReporter* eventReporter_;
	PxDefaultErrorCallback* errorCallback_; //unused atm

	///////////////////////////////////////////////////////////////////////////

	//singleton pattern
	static PhysicsSystemManager* instance_;
	void setupInstance();
	void shutdownInstance();

	//private constructor
	PhysicsSystemManager() { setupInstance(); };
	~PhysicsSystemManager() { shutdownInstance(); };
};

#endif /* PHYS_SYS_MANAGER_H_ */