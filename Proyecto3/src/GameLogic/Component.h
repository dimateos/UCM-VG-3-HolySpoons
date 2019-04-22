//Nap_Time_Studios
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "GameObject.h"

#include "Listener.h"
// every type of component inherites from this class
class Component : public Listener, public Activable, public Identifiable, public Initiable
{
public:
	inline Component() : Activable(), Identifiable(), cfg_(), owner_(nullptr) {};
	inline Component(nap_json const & cfg) :
		Activable(FINDnRETURN(cfg, activeString, bool, true)), Identifiable(cfg["id"]), cfg_(cfg), owner_(nullptr) {};
	inline Component(nap_json const & cfg, GameObject* owner)
		: Activable(FINDnRETURN(cfg, activeString, bool, true)), Identifiable(cfg["id"]), cfg_(cfg), owner_(owner) {};
	inline virtual ~Component() {};

	// called from gameObjects themselves
	// non pure so you dont have to create empty functions
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt) { return false; };
	virtual void update(GameObject* o, double time) {};
	virtual void late_update(GameObject* o, double time) {};

	// it sets the cfg and the owner
	virtual inline void Init(nap_json const & cfg, GameObject* owner) {
		setCfg(cfg);
		setOwner(owner);
	}

	inline void setOwner(GameObject* owner) { owner_ = owner; }
	inline GameObject* getOwner() { return owner_; };

	//some debugging reading the config
	inline void setCfg(nap_json const & cfg) {
		active_ = FINDnRETURN(cfg, activeString, bool, true);
		setId(cfg["id"]);
		cfg_ = cfg;
	}
	inline nap_json & getCfg() { return cfg_; }

protected:
	nap_json cfg_;		//for async setup
	GameObject* owner_; //the go owner
};

#endif /* COMPONENT_H_ */