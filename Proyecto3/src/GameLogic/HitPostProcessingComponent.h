#ifndef TIMEDPOSTPROCESSINGCOMPONENT_H_
#define TIMEDPOSTPROCESSINGCOMPONENT_H_

#include "Component.h"
class HitPostProcessingComponent: public Component
{
private:
	std::string compositorName;
	bool currentlyActive = false;

	void setEnableComp(bool b = true);

public:
	virtual ~HitPostProcessingComponent() {};
	HitPostProcessingComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual void setUp();

	virtual void receive(Message* msg);
};

#endif //TIMEDPOSTPROCESSINGCOMPONENT_H_