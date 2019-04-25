//Nap_Time_Studios
#pragma once

#include <PxSimulationEventCallback.h>
class EventReporter : public physx::PxSimulationEventCallback
{
public:
	// Custom event filter shader = anything reports (maybe should remove some)
	static physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);

	//for now just collisions
	void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);

	//unused
	void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(physx::PxActor** actors, physx::PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const physx::PxRigidBody*const*, const physx::PxTransform*, const physx::PxU32) {}
};