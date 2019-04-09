#ifndef FLOATINGENEMYCOMPONENT_H_
#define FLOATINGENEMYCOMPONENT_H_

#include "Component.h"

class FloatingEnemyComponent :
	public Component
{
private:
	void WaveyMovement(float t);

	//logic
	float initY; //initial Y, needed sin movement
	float timer = 0, rotation = 0;
	nap_transform* dest;

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

#endif /* FLOATINGENEMYCOMPONENT_H_ */
