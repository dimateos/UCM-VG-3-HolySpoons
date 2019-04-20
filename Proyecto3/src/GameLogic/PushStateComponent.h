#ifndef PushStateComponent_H_
#define PushStateComponent_H_

#include "Component.h"

class PushStateComponent : public Component
{
private:
	SDL_Keycode key = SDLK_8; //READ IT FROM JSON?
	std::string state;
	std::string json;

public:
	PushStateComponent() {};
	virtual ~PushStateComponent() {};
	PushStateComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
};


#endif //PushStateComponent_H_