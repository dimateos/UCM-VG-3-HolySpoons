#ifndef TIMEDPOSTPROCESSINGCOMPONENT_H_
#define TIMEDPOSTPROCESSINGCOMPONENT_H_

#include "Component.h"
#include <Timer.h> //invunerability

class TimedPostProcessingComponent: public Component
{
private:
	std::string compositorName;
	bool currentlyActive = false;
	napTimer timer;

public:
	virtual ~TimedPostProcessingComponent() {};
	TimedPostProcessingComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void update(GameObject* ent, double time);
	virtual void receive(Message* msg);
};

#endif //TIMEDPOSTPROCESSINGCOMPONENT_H_