//Nap_Time_Studios
#ifndef TEST_COMP_H_
#define TEST_COMP_H_

#include "Component.h"
//tmp proof of concepts
class TestComponent : public Component
{
public:
	inline TestComponent() { };
	inline TestComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~TestComponent() { };
	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);

protected:
};

#endif /* TEST_COMP_H_ */
