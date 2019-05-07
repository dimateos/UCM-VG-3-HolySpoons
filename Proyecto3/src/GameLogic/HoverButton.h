#ifndef HOVERBUTTON_H_
#define HOVERBUTTON_H_

//fowarding
namespace Ogre {
	class OverlayElement;
}

#include "Component.h"
class HoverButton : public Component
{
public:
	inline HoverButton(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~HoverButton() {};
	virtual void setUp();

private:
	double X, Y, W, H;
	double bigX, bigY, bigW, bigH;
	int enterMsg, exitMsg;
	std::string listener;
	Ogre::OverlayElement* elemt;

	bool outside = true;

	bool inside(int x, int y);
	void onClick();
	void centerOverlay();

	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif /* HOVERBUTTON_H_ */
