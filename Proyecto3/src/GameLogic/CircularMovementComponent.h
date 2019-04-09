//Nap_Time_Studios
#ifndef CIRCULARMOVEMENTCOMPONENT_H_
#define CIRCULARMOVEMENTCOMPONENT_H_
#include "Component.h"

class CircularMovementComponent :
	public Component
{
private:
	void CircularMovement(float t);

	//logic
	float timer = 0, rotation = 0;
	nap_transform* dest;

	//cfg (json)
	float speed = 0.1, rad = 50;

public:
	/*
	 *Default constructor
	 */
	inline CircularMovementComponent() {};
	/*
	 *Owner needed
	 */
	inline CircularMovementComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~CircularMovementComponent() {};

	/*
	 *Virtual setup, inits component
	*/
	virtual void setUp();

	virtual void update(GameObject* ent, double time);
};

#endif /* CIRCULARMOVEMENTCOMPONENT_H_ */

