#pragma once
#include "Component.h"
#include <RenderSystemInterface.h>

class AutoRotationComponent :
	public Component
{
private:
	GameObject* o_;
	nap_vector3 dest = {0, 15, 0};
	const nap_vector3 DIRECTOR = {0, 0, 1};

public:
	inline AutoRotationComponent() {};
	inline AutoRotationComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual void setUp();

	virtual void update(GameObject* ent, double time);

	virtual ~AutoRotationComponent();
};

