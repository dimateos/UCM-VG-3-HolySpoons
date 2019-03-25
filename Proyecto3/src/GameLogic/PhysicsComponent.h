//Nap_Time_Studios
#ifndef PHYS_COMP_H_
#define PHYS_COMP_H_

#include "PhysicsSystemManager.h"

#include "Listener.h"
#include "Component.h"
class PhysicsComponent : public Component, public Listener
{
public:
	inline PhysicsComponent(nap_json const & cfg) : Component(cfg) { this->setUp(cfg); };
	inline PhysicsComponent(nap_json const & cfg, nap_transform *trans) : Component(cfg) { this->setUp(cfg); setUserData(trans); };
	inline virtual ~PhysicsComponent() { setDown(); };

	//physx userData operations
	void setUserData(nap_transform * trans);
	nap_transform* getUserData();

	virtual void receive(Message* msg);

	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	virtual void update(GameObject* ent, float time);
	virtual void late_update(GameObject* ent, float time);

protected:
	virtual void setUp(nap_json const & cfg);
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
