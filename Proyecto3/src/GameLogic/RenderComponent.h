//Nap_Time_Studios
#ifndef RENDER_COMP_H_
#define RENDER_COMP_H_

#include "RenderSystemInterface.h"
#define ogre_scale 100

#include "Component.h"
class RenderComponent : public Component
{
public:
	inline RenderComponent(nap_json const & cfg) : Component(cfg) { this->setUp(cfg); };
	inline virtual ~RenderComponent() { setDown(); };

	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	virtual void update(GameObject* ent, float time);
	virtual void late_update(GameObject* ent, float time);

protected:
	virtual void setUp(nap_json const & cfg);
	virtual void setDown();

	//Assist on setting up the shape
	OgrePair getOgrePair(nap_json shape);

	Ogre::SceneNode *node;
	Ogre::Entity *entity;
};

#endif /* RENDER_COMP_H_ */
