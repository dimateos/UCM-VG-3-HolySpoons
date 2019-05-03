//Nap_Time_Studios
#ifndef CAMERAMENU_H_
#define CAMERAMENU_H_

#include "RenderSystemInterface.h"

#include "Component.h"
class CameraMenu : public Component
{
public:
	inline CameraMenu(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~CameraMenu() {};
	virtual void setUp();
	virtual void lateSetUp();

	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) { return false; }
	virtual void update(GameObject* ent, double time) {}

protected:
	Ogre::SceneNode* camNode_;
};

#endif /* CAMERAMENU_H_ */