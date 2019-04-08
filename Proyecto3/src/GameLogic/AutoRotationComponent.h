#pragma once
#include "Component.h"
#include <RenderSystemInterface.h>

class AutoRotationComponent :
	public Component
{
private:
	GameObject* o_;
	nap_transform* dest;  //pointer to dest position
	const nap_vector3 DIRECTOR = {0, 0, 1}; //DEFAULT, ALL OBJECTS MUST BE LOOKING AT THE CAMERA

public:
	inline AutoRotationComponent() {};
	inline AutoRotationComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual void setUp();

	virtual void update(GameObject* ent, double time);

	virtual ~AutoRotationComponent();
};

