#pragma once
#include "Component.h"

class FloatingEnemyComponent :
	public Component
{
private:
	void WaveyMovement(float t);

	//logic
	nap_vector3  initPos;
	float initY;
	float timer = 0, rotation = 0;
	nap_vector3 destPos = {30, 0, 0};

	//cfg (json)
	float amplitude = 1, horiSpeed = 0.01, vertSpeed = 1;

public:
	inline FloatingEnemyComponent() {};
	inline FloatingEnemyComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~FloatingEnemyComponent() {};

	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

