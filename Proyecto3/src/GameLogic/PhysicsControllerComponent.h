//Nap_Time_Studios
#ifndef PHYS_CONTROLLER_COMP_H_
#define PHYS_CONTROLLER_COMP_H_

#include "PhyscsFowarder.h"

#include "Component.h"
class PhysicsControllerComponent : public Component
{
public:
	inline PhysicsControllerComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~PhysicsControllerComponent() { setDown(); };
	virtual void setUp();

	inline PxController * getController() const { return controller_; };
	void invalidateChache();

	//interacting with the controller movement
	inline nap_vector3 getV() const { return v_; };
	inline void setV(nap_vector3 v) { v_ = v; };
	inline nap_vector3 getA() const { return a_; };
	inline void setA(nap_vector3 a) { a_ = a; };
	inline nap_vector3 getI() const { return i_; };
	inline void addI(nap_vector3 i) { i_ = i_ + i; };
	inline nap_vector3 getG() const { return g_; };
	inline void setG(nap_vector3 g) { g_ = g; };

	virtual void update(GameObject* ent, double time);
	virtual void late_update(GameObject* o, double time);
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);

protected:
	virtual void setDown();
	virtual void configActive();

	ID customTestID;

	//sync GO transforms with px
	nap_userData * ud_ = nullptr;
	void updateUserData();

	//Controller stuff
	PxController* controller_;
	PxRigidActor* getActor();

	//Custom movement for the controller
	nap_vector3 v_, a_, i_, g_;
	float damping_, minDist, i_mass;
};

#endif /* PHYS_CONTROLLER_COMP_H_ */
