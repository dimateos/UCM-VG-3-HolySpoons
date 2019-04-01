//Nap_Time_Studios
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <SDL_events.h>	//events

#include <list>	//components
using namespace std;

#include "Transforms.h"
#include "Activable.h"
#include "Identifiable.h"
#include "Initiable.h"

class Component;

// this class accepts components (empty container waiting for functionality)
class GameObject : public Activable, public Identifiable, public Initiable
{
public:
	GameObject(nap_json const & cfg);
	GameObject(nap_json const & cfg, std::list<Component*>);
	virtual ~GameObject();

	//setup on gamestate push (awake)
	virtual void setUp();

	//latesetup on gamestate (start)
	virtual void lateSetUp();

	//MAIN methods (call components)
	virtual bool handleEvents(const SDL_Event evt);
	virtual void update(float time);
	virtual void late_update(float time);

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
	inline void setPosition(nap_vector3 p) { trans_.p_ = p; setUpToDate(false); }
	inline nap_vector3 getPosition() const { return trans_.p_; }
	inline void setOrientation(nap_quat q) { trans_.q_ = q; setUpToDate(false); }
	inline nap_quat getOrientation() const { return trans_.q_; }
	inline void setTransform(nap_vector3 p, nap_quat q) { trans_.p_ = p; trans_.q_ = q; setUpToDate(false); }
	//UpToDate flags
	inline void setUpToDate(bool b = true) { trans_.upToDate_phys = b; trans_.upToDate_rend = b; }
	inline void setUpToDate_phys(bool b = true) { trans_.upToDate_phys = b; }
	inline bool getUpToDate_phys() const { return trans_.upToDate_phys; }
	inline void setUpToDate_rend(bool b = true) { trans_.upToDate_rend = b; }
	inline bool getUpToDate_rend() const { return trans_.upToDate_rend; }
	//to keep track of the transform (Physx, IA, etc)
	inline nap_transform* getTransPtr() { return &trans_; }

protected:
	nap_json cfg_; //for async setup

	nap_transform trans_;
	std::list<Component*> components_; // component list
};

#endif /* GAMEOBJECT_H_ */

