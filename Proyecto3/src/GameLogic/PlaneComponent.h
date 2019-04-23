//Nap_Time_Studios
#ifndef PLANE_COMP_H_
#define PLANE_COMP_H_

#include "Component.h"

class PlaneComponent : public Component
{
public:
	inline PlaneComponent() { };
	inline PlaneComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~PlaneComponent() { };
	virtual void setUp();

	virtual void update(GameObject* o, double time) {}
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt) { return false; }
};

#endif /* PLANE_COMP_H_ */
