//Nap_Time_Studios
#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include "RenderSystemInterface.h"

#include "Component.h"
class FPSCamera : public Component
{
public:
	inline FPSCamera() {};
	inline FPSCamera(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~FPSCamera() {};
	virtual void setUp();
	virtual void lateSetUp();

	//mouse move + rightClick and screen resize
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	//smooth movement -> updates all the time
	virtual void update(GameObject* ent, double time);
	virtual void receive(Message * msg);

protected:
	void toggleZoom();

	//logic
	Ogre::SceneNode* camNode_;
	float rotX_, rotY_, total_rotY_; //updaters
	bool zoom = false;

	nap_vector3 relativePos;

	//cfg (json)
	float rotXspeed_, rotYspeed_, zoomed_, maxRotY_;
};

#endif /* FPSCAMERA_H_ */