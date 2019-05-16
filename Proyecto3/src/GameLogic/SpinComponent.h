//Nap_Time_Studios
#ifndef SPINCOMPONENT_H_
#define SPINCOMPONENT_H_

#include "Component.h"

namespace Ogre {
	class SceneNode;
};

class SpinComponent :
	public Component
{
private:
	Ogre::SceneNode* node = nullptr;
	float roll_ = 0.0f, pitch_ = 0.0f, yaw_ = 0.0f;
	void rotate(GameObject * ent, double time);

public:
	inline SpinComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~SpinComponent();
	virtual void setUp();
	virtual void lateSetUp();

	virtual void late_update(GameObject* ent, double time);

};

#endif /* SPINCOMPONENT_H_ */

