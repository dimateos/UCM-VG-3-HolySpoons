#include "Component.h"

#ifndef ButtonComponent_H_
#define ButtonComponent_H_

namespace Ogre {
	class OverlayElement;
}
class ButtonComponent : public Component
{
public:
	ButtonComponent() {};
	ButtonComponent(nap_json const & cfg, GameObject* owner);
	~ButtonComponent() {};

private:
	double X, Y, W, H;
	double bigX, bigY, bigW, bigH;
	int msgType;
	std::string panelName;
	Ogre::OverlayElement* elemt;

	void OnClick();
	void setUp();
	
	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif
