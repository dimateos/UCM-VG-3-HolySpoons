//Nap_Time_Studios
#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "GameObject.h"

// every type of component inherites from this class
class Component : public Activable, public Identifiable
{
public:
	inline Component() {};
	inline Component(nap_json const & cfg) : Activable(), Identifiable(cfg.find("name") != cfg.end() ? cfg["name"] : "") {};
	inline virtual ~Component() {};

	// called from gameObjects themselves
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) = 0;
	virtual void update(GameObject* ent, float time) = 0;
	virtual void late_update(GameObject* ent, float time) {};

protected:
	// some structure around setUp / down
	inline virtual void setUp(nap_json const & cfg) {};
	inline virtual void setDown() {};

};

#endif /* COMPONENT_H_ */