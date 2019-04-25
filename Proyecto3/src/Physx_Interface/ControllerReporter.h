//Nap_Time_Studios
#pragma once

#include <characterkinematic\PxController.h>
using namespace physx;

class nap_userData; //fowarding

class ControllerReporter : public PxUserControllerHitReport
{
public:
	inline ControllerReporter(nap_userData ** ud) : ud_(ud) {};
	virtual void onShapeHit(const PxControllerShapeHit& hit);

	//unused
	virtual void onControllerHit(const PxControllersHit& hit) { PX_UNUSED(hit); };
	virtual void onObstacleHit(const PxControllerObstacleHit& hit) { PX_UNUSED(hit); };

protected:
	nap_userData ** ud_;
};
