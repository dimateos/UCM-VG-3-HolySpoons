//Nap_Time_Studios
#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <SDL_events.h>	//events
#include "json.hpp"
using nap_json = nlohmann::json;

#include <list>	//components
using namespace std;

#include "Transforms.h"
#include "Emitter.h"
#include "Activable.h"
#include "Identifiable.h"
class Component;

// this class accepts components (empty container waiting for functionality)
class GameObject : public Emitter, public Activable, public Identifiable
{
public:
	GameObject(nap_json const & cfg);
	GameObject(nap_json const & cfg, std::list<Component*>);
	GameObject(nap_json const & cfg, std::list<Component*>, std::list<Listener*>);
	virtual ~GameObject();

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
	inline void setPosition(nap_vector3 p) { trans.p_ = p; trans.updateState_ = userUpdated; }
	inline nap_vector3 getPosition() const { return trans.p_; }
	inline void setOrientation(nap_quat q) { trans.q_ = q; trans.updateState_ = userUpdated; }
	inline nap_quat getOrientation() const { return trans.q_; }
	inline void setTransform(nap_vector3 p, nap_quat q) { trans.p_ = p; trans.q_ = q; trans.updateState_ = userUpdated; }
	//updateState flag
	inline void setUpdateState(updateState s) { trans.updateState_ = s; }
	inline updateState getUpdateState() const { return trans.updateState_; }
	//to keep track of the transform (Physx, IA, etc)
	inline nap_transform* getTransPtr() { return &trans; }

private:
	virtual void setUp(nap_json const & cfg);

	nap_transform trans;
	std::list<Component*> components_; // component list
};

#endif /* GAMEOBJECT_H_ */

