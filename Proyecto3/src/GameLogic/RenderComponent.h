//Nap_Time_Studios
#ifndef RENDER_COMP_H_
#define RENDER_COMP_H_

#include "RenderSystemInterface.h"

#include "Component.h"

class RenderComponent : public Component
{
public:
	inline RenderComponent() {}
	inline RenderComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~RenderComponent() { setDown(); };
	virtual void setUp();

	inline Ogre::SceneNode* getSceneNode() { return node; }

	virtual void update(GameObject* ent, double time);
	virtual void late_update(GameObject* ent, double time);

protected:
	virtual void setDown();
	virtual void configActive();

	//used to optimize synchronization on late_update when not required
	bool ignoreTrans_, updateOri_, invisible_;

	//Assist on setting up the shape
	OgrePair getOgrePair(nap_json shape);

	Ogre::SceneNode *node;
	Ogre::Entity *entity;

	Ogre::AnimationState* idleAnimation;
};

#endif /* RENDER_COMP_H_ */
