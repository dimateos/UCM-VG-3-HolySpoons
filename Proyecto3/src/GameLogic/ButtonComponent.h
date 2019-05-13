#ifndef ButtonComponent_H_
#define ButtonComponent_H_

//fowarding
namespace Ogre {
	class OverlayElement;
}

#include "Component.h"
class ButtonComponent : public Component
{
public:
	inline ButtonComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~ButtonComponent();
	virtual void setUp();
	Ogre::OverlayElement* getElement() { return elemt; };

private:
	double X, Y, W, H;
	double bigX, bigY, bigW, bigH;
	int msgType;
	std::string listener;
	Ogre::OverlayElement* elemt;

	bool inside(int x, int y);
	void onClick();
	void centerOverlay();

	bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif /* ButtonComponent_H_ */
