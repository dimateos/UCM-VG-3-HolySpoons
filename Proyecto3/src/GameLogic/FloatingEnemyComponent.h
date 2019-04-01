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

	void WaveyMovement(float t);


	float amplitude = 1; 
	float horiSpeed = 0.01;
	float vertSpeed = 1;
	float initY;
	float timer = 0;
	float rotation = 0;
	nap_vector3  initPos;

public:
	FloatingEnemyComponent() {};
	FloatingEnemyComponent(GameObject* o, nap_json const & cfg) : Component(cfg), o(o) { this->setUp(); };
	virtual ~FloatingEnemyComponent() {};

	virtual void setUp();
	virtual void lateSetUp();

	virtual void update(GameObject* ent, float time);
};

