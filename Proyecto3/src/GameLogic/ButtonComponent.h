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
	inline virtual ~ButtonComponent() {};
	virtual void setUp();

private:
	double X, Y, W, H;
	double bigX, bigY, bigW, bigH;
	int msgType;
	std::string panelName;
	Ogre::OverlayElement* elemt;

	void OnClick();
	void centerOverlay();

	bool handleEvents(GameObject* o, const SDL_Event& evt);
	bool inside(int x, int y);
};

#endif /* ButtonComponent_H_ */
