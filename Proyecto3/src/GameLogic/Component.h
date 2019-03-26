//Nap_Time_Studios
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "GameObject.h"
#include "Listener.h"

// every type of component inherites from this class
class Component : public Listener, public Activable, public Identifiable, public Initiable
{
public:
	inline Component() : Activable(), Identifiable(), cfg_() {};
	inline Component(nap_json const & cfg) : Activable(), Identifiable(cfg["id"]), cfg_(cfg) {};
	inline virtual ~Component() {};

	// called from gameObjects themselves
	// non pure so you dont have to create empty functions
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt) { return false; };
	virtual void update(GameObject* o, float time) {};
	virtual void late_update(GameObject* o, float time) {};

protected:
	nap_json cfg_; //for async setup
};

#endif /* COMPONENT_H_ */