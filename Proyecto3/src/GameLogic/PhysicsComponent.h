//Nap_Time_Studios
#ifndef PHYS_COMP_H_
#define PHYS_COMP_H_

#include "PhysicsSystemManager.h"

#include "Listener.h"
#include "Component.h"
class PhysicsComponent : public Component, public Listener
{
public:
	inline PhysicsComponent(nap_json const & cfg) : Component(cfg) { };
	inline virtual ~PhysicsComponent() { setDown(); };
	virtual void setUp();

	//physx userData operations
	void setUserData(nap_transform * trans_);
	nap_transform* getUserData();

	virtual void receive(Message* msg);
	virtual void late_update(GameObject* ent, float time);

protected:
	virtual void setDown();

	//Assist on setting up the shape
	PxGeometry * getShape(nap_json shape);
	//static const map<std::string, PxGeometryType::Enum> geoTypes;

	//RigidBody config
	PxRigidDynamic* rigidBodyD = nullptr;
	PxRigidStatic* rigidBodyS = nullptr;
	bool dynamic = true;
	inline PxRigidActor* getActor() const {
		if (dynamic) return rigidBodyD; else return rigidBodyS;
	};
};

#endif /* PHYS_COMP_H_ */
