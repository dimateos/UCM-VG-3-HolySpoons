//Nap_Time_Studios
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <list>	//components
using namespace std;

//fowaring
class Component;
class nap_transform;
class nap_vector3;
class nap_quat;
class nap_scale;

typedef union SDL_Event;

#include "Activable.h"
#include "Identifiable.h"
#include "Initiable.h"
#include "CollisionListener.h"

// this class accepts components (empty container waiting for functionality)
class GameObject : public Activable, public Identifiable, public Initiable, public CollisionListenersHolder
{
public:
	GameObject(nap_json const & cfg);
	GameObject(nap_json const & cfg, std::list<Component*>);
	GameObject(GameObject* o);
	virtual ~GameObject();

	//setup on gamestate push (awake)
	virtual void setUp();

	//latesetup on gamestate (start)
	virtual void lateSetUp();

	//MAIN methods (call components)
	virtual bool handleEvents(const SDL_Event evt);
	virtual void update(double time);
	virtual void late_update(double time);

	//components operations
	void addComponent(Component* component);
	void addComponent(std::list<Component*> comps);
	void delComponent(std::string name);
	void delComponent(Component* component);
	void delComponent(std::list<Component*> comps);
	Component* getComponent(std::string name);
	std::list<Component*> getComponents();
	void clearComponents();

	//transform operations (set's activates the flags)
	inline void setPosition(nap_vector3 p) { trans_.p_ = p; setTransUpToDate(false); }
	inline nap_vector3 getPosition() const { return trans_.p_; }
	inline void setOrientation(nap_quat q) { trans_.q_ = q; setTransUpToDate(false); }
	inline nap_quat getOrientation() const { return trans_.q_; }
	inline void setTransform(nap_vector3 p, nap_quat q) { trans_.p_ = p; trans_.q_ = q; setTransUpToDate(false); }
	//transfotm UpToDate flags
	inline void setTransUpToDate(bool b = true) { trans_.upToDate_phys = b; trans_.upToDate_rend = b; }
	inline void setTransUpToDate_phys(bool b = true) { trans_.upToDate_phys = b; }
	inline bool getTransUpToDate_phys() const { return trans_.upToDate_phys; }
	inline void setTransUpToDate_rend(bool b = true) { trans_.upToDate_rend = b; }
	inline bool getTransUpToDate_rend() const { return trans_.upToDate_rend; }
	//to keep track of the transform (Physx, IA, etc)
	inline nap_transform* getTransPtr() { return &trans_; }

	//scale operations
	inline void setScale(nap_vector3 s) { scale_.s_ = s; setScaleUpToDate(false); }
	inline nap_vector3 getScale() const { return scale_.s_; }
	//scale UpToDate flags
	inline void setScaleUpToDate(bool b = true) { scale_.upToDate_phys = b; scale_.upToDate_rend = b; }
	inline void setScaleUpToDate_phys(bool b = true) { scale_.upToDate_phys = b; }
	inline bool getScaleUpToDate_phys() const { return scale_.upToDate_phys; }
	inline void setScaleUpToDate_rend(bool b = true) { scale_.upToDate_rend = b; }
	inline bool getScaleUpToDate_rend() const { return scale_.upToDate_rend; }
	//to keep track of the scale (Physx, IA, etc)
	inline nap_scale* getScalePtr() { return &scale_; }

	//some debugging reading the config
	inline nap_json & getCfg() { return cfg_; }

protected:
	void setDown();
	virtual void configActive();

	nap_json cfg_; //for async setup

	nap_transform trans_;
	nap_scale scale_;
	std::list<Component*> components_; // component list
};

#endif /* GAMEOBJECT_H_ */

