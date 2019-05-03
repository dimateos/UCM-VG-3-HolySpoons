#ifndef PopStateComponent_H_
#define PopStateComponent_H_

#include "Component.h"
class PopStateComponent : public Component
{
private:
	SDL_Keycode key;
	std::string state;
	void popState();

public:
	inline PopStateComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~PopStateComponent() {};
	virtual void setUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void receive(Message * msg);
};

#endif //PopStateComponent_H_