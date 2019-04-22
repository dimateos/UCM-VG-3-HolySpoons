#include "Component.h"

#ifndef BUTTON_H_
#define BUTTON_H_

class Button : public Component
{
public:
	Button() {};
	Button(nap_json const & cfg, GameObject* owner);
	~Button() {};

private:
	int X, Y, W, H;
	int msgType;
	std::string panelName;

	void OnClick();
	void setUp();

	
	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif
