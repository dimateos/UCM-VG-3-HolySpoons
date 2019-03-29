//Nap_Time_Studios
#ifndef PHYS_COMP_H_
#define PHYS_COMP_H_

#include "PhysicsSystemManager.h"

#include "Component.h"
class PhysicsComponent : public Component
{
public:
	inline PhysicsComponent(nap_json const & cfg) : Component(cfg) {};
	inline PhysicsComponent(nap_json const & cfg, nap_transform * trans) : Component(cfg) { setUserData(trans); };
	inline virtual ~PhysicsComponent() { setDown(); };
	virtual void setUp();

	//physx userData operations
	void setUserData(nap_transform * trans);
	nap_transform* getUserData();

	PxRigidDynamic * getDynamicBody() const { return rigidBodyD_; };

	virtual void receive(Message* msg);
	virtual void late_update(GameObject* o, float time);

protected:
	virtual void setDown();

	//Assist on setting up the shape
	PxGeometry * getShape(nap_json shape);
	//static const map<std::string, PxGeometryType::Enum> geoTypes;

	void updateUserData();
	nap_transform * user_trans_;

	//RigidBody config
	PxRigidDynamic* rigidBodyD_ = nullptr;
	PxRigidStatic* rigidBodyS_ = nullptr;
	bool dynamic = true;
	inline PxRigidActor* getActor() const {
		if (dynamic) return rigidBodyD_; else return rigidBodyS_;
	};
};

#endif /* PHYS_COMP_H_ */
