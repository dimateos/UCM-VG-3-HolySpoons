//Nap_Time_Studios
#ifndef RENDER_COMP_H_
#define RENDER_COMP_H_

#include "RenderSystemInterface.h"
#define ogre_scale 100

#include "Component.h"
class RenderComponent : public Component
{
public:
	inline RenderComponent(nap_json const & cfg) : Component(cfg) { };
	inline virtual ~RenderComponent() {};
	virtual void setUp();

	virtual void late_update(GameObject* ent, float time);

protected:
	//Assist on setting up the shape
	OgrePair getOgrePair(nap_json shape);

	Ogre::SceneNode *node;
	Ogre::Entity *entity;
};

#endif /* RENDER_COMP_H_ */
