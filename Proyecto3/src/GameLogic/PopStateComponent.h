#ifndef PopStateComponent_H_
#define PopStateComponent_H_

#include <SDL_events.h>

#include "Component.h"
class PopStateComponent : public Component
{
private:
	SDL_Keycode key = SDLK_8; //READ IT FROM JSON?
	std::string state;
public:
	PopStateComponent() {};
	virtual ~PopStateComponent() {};
	PopStateComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
};


#endif //PopStateComponent_H_