#pragma once
#include "Component.h"

class FloatingEnemyComponent :
	public Component
{
private:
	GameObject* o;

	void WaveyMovement(float t);

	//logic
	nap_vector3  initPos;
	float initY;
	float timer = 0, rotation = 0;

	//cfg (json)
	float amplitude = 1, horiSpeed = 0.01, vertSpeed = 1;

public:
	FloatingEnemyComponent() {};
	FloatingEnemyComponent(GameObject* o, nap_json const & cfg) : Component(cfg), o(o) {};
	virtual ~FloatingEnemyComponent() {};

	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

