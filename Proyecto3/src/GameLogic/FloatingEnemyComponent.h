#pragma once
#include "Component.h"

namespace physx {
	class PxRigidDynamic;
};

class FloatingEnemyComponent :
	public Component
{
private:
	GameObject* o;
	physx::PxRigidDynamic* rigidBody;

	void WaveyMovement();

public:
	FloatingEnemyComponent() {};
	inline FloatingEnemyComponent(GameObject* o, nap_json const & cfg) : Component(cfg), o(o) { this->setUp(); };
	virtual ~FloatingEnemyComponent() {};

	virtual void setUp();
	virtual void lateSetUp();

	virtual void update(GameObject* ent, float time);
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);

};

