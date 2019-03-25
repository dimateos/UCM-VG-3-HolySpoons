//Nap_Time_Studios
#ifndef TEST_COMP_H_
#define TEST_COMP_H_

#include "Component.h"
//tmp proof of concepts
class TestComponent : public Component
{
public:
	inline TestComponent() { this->setUp(nap_json()); };
	inline TestComponent(nap_json const & cfg) : Component(cfg) { this->setUp(cfg); };
	inline virtual ~TestComponent() { setDown(); };

	virtual void update(GameObject* ent, float time);
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt);
	virtual void late_update(GameObject* ent, float time);

protected:
	virtual void setUp(nap_json const & cfg);
	virtual void setDown();
};

#endif /* TEST_COMP_H_ */
