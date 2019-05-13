#ifndef BUTTONSIZECHANGECOMPONENT_H_
#define BUTTONSIZECHANGECOMPONENT_H_

//fowarding
namespace Ogre {
	class OverlayElement;
}

#include "Component.h"
class ButtonSizeChangeComponent : public Component
{
public:
	inline ButtonSizeChangeComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~ButtonSizeChangeComponent() {};
	virtual void setUp();
	virtual void lateSetUp();

private:
	double X, Y, W, H;
	double bigX, bigY, bigW, bigH;
	float multiplier;
	//int enterMsg, exitMsg;
	std::string buttonMat = "", imgMat = "";
	std::string listener;
	Ogre::OverlayElement* elemt;

	bool outside = true;

	bool inside(int x, int y);
	void onClick();
	void centerOverlay();

	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif /* BUTTONSIZECHANGECOMPONENT_H_ */
