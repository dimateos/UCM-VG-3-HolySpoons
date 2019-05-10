#ifndef DAMAGETICKCOMPONENT_H_
#define DAMAGETICKCOMPONENT_H_

#include "Component.h"
#include <Timer.h> //invunerability

class GameObject;

class DamageTickComponent: public Component
{
private:
	napTimer timer;
	GameObject* target_ = nullptr;
	int dmg;
	int enterId,exitId;

public:
	virtual ~DamageTickComponent() {};
	DamageTickComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void receive(Message* msg);
	virtual void update(GameObject * ent, double time);
};

#endif //DAMAGETICKCOMPONENT_H_