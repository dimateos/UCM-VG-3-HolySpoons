#pragma once
#include "Component.h"
#include <RenderSystemInterface.h>

class AutoRotationComponent :
	public Component
{
private:
	GameObject* o_;
	nap_vector3 dest = {5, 5, 40};

public:
	AutoRotationComponent() {};
	AutoRotationComponent(nap_json const & cfg) : Component(cfg) { this->setUp(); };
	virtual void setUp();

	virtual void update(GameObject* ent, float time);

	virtual ~AutoRotationComponent();
};

