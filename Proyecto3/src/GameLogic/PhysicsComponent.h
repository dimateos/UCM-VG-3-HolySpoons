//Nap_Time_Studios
#ifndef PHYS_COMP_H_
#define PHYS_COMP_H_

#include "PhyscsFowarder.h"

#include "Component.h"
class PhysicsComponent : public Component
{
public:
	inline PhysicsComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~PhysicsComponent() { setDown(); };
	virtual void setUp();

	PxRigidDynamic * getDynamicBody();
	void setShapeTrigger(bool b = true);

	virtual void late_update(GameObject* o, double time);

protected:
	virtual void setDown();
	virtual void configActive();

	//used to optimize synchronization on late_update when not required
	bool ignoreTrans_, updateOri_;

	//sync GO transforms with px
	nap_userData * ud_ = nullptr;
	void updateUserData();

	//Assist on setting up the shape
	PxGeometry * getGeo(nap_json shape);
	PxShape * shape_ = nullptr;

	//RigidBody config
	PxRigidDynamic* rigidBodyD_ = nullptr;
	PxRigidStatic* rigidBodyS_ = nullptr;
	PxRigidActor* getActor();
};

#endif /* PHYS_COMP_H_ */
