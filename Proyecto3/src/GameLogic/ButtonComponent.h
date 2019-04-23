#include "Component.h"

#ifndef ButtonComponent_H_
#define ButtonComponent_H_

class ButtonComponent : public Component
{
public:
	ButtonComponent() {};
	ButtonComponent(nap_json const & cfg, GameObject* owner);
	~ButtonComponent() {};

private:
	int X, Y, W, H;
	int msgType;
	std::string panelName;

	void OnClick();
	void setUp();
	
	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif
