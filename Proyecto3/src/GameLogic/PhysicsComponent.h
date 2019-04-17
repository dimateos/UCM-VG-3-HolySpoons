//Nap_Time_Studios
#ifndef PHYS_COMP_H_
#define PHYS_COMP_H_

#include "PhysicsSystemManager.h"

#include "Component.h"
class PhysicsComponent : public Component
{
public:
	inline PhysicsComponent() {}
	inline PhysicsComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~PhysicsComponent() { setDown(); };
	virtual void setUp();

	inline PxRigidDynamic * getDynamicBody() const { return rigidBodyD_; };

	virtual void late_update(GameObject* o, double time);

protected:
	virtual void setDown();
	virtual void configActive();

	//sync GO transforms with px
	nap_userData * ud = nullptr;
	void updateUserData();

	//Assist on setting up the shape
	PxGeometry * getGeo(nap_json shape);
	PxShape * shape_;

	//RigidBody config
	PxRigidDynamic* rigidBodyD_ = nullptr;
	PxRigidStatic* rigidBodyS_ = nullptr;
	inline PxRigidActor* getActor() const {
		if (rigidBodyD_ != NULL && rigidBodyD_ != nullptr) return rigidBodyD_;
		else return rigidBodyS_;
	};
};

#endif /* PHYS_COMP_H_ */
