#ifndef PushStateComponent_H_
#define PushStateComponent_H_

#include "Component.h"
class PushStateComponent : public Component
{
private:
	SDL_Keycode key;
	std::string state;
	std::string json;
	void pushState();

public:
	inline virtual ~PushStateComponent() {};
	inline PushStateComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual void setUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void receive(Message* msg);
};


#endif //PushStateComponent_H_