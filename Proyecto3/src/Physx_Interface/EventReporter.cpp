#include "EventReporter.h"

void EventReporter::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
{
	PX_UNUSED(pairs);
	PX_UNUSED(nbPairs);
	physx::PxActor* actor1 = pairHeader.actors[0];
	physx::PxActor* actor2 = pairHeader.actors[1];

	//send messages or something
}

//physx::PxFilterFlags contactReportFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
//	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
//	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
//{
//	PX_UNUSED(attributes0);
//	PX_UNUSED(attributes1);
//	PX_UNUSED(filterData0);
//	PX_UNUSED(filterData1);
//	PX_UNUSED(constantBlockSize);
//	PX_UNUSED(constantBlock);
//
//	// all initial and persisting reports for everything, with per-point data
//	pairFlags = physx::PxPairFlag::eSOLVE_CONTACT | physx::PxPairFlag::eDETECT_DISCRETE_CONTACT
//		| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
//		| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS
//		| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
//	return physx::PxFilterFlag::eDEFAULT;
//}