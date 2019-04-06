#pragma once
#include "Component.h"

class FloatingEnemyComponent :
	public Component
{
private:
	void WaveyMovement(float t);

	//logic
	float initY; //initial Y, needed sin movement
	float timer = 0, rotation = 0;
	nap_vector3 destPos = {30, 0, 0};

	//cfg (json)
	float amplitude = 1, horiSpeed = 0.01, vertSpeed = 1;

public:
	/*
	 *Default constructor
	 */
	inline FloatingEnemyComponent() {};
	/*
	 *Owner needed
	 */
	inline FloatingEnemyComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~FloatingEnemyComponent() {};

	/*
	 *Virtual setup, inits component
	*/
	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

