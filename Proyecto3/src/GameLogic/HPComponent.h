//Nap_Time_Studios
#ifndef HP_COMPONENT_H_
#define HP_COMPONENT_H_

#include "Component.h"
class HPComponent : public Component
{
private:
	int InitHP, incrementHP;
	int HP;
	string soundEmitter;

public:
	HPComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), InitHP(0), HP(0) {}
	virtual ~HPComponent() {}
	virtual void setUp();

	inline int getHP() const { return HP; }
	inline void resetHP() { HP = InitHP; }
	inline void subHP(int damage) { HP -= damage; if (HP < 0) HP = 0; }
	inline void addHP(int health) { HP += health; }
	inline int const getInitHP() const { return InitHP; };

	virtual void receive(Message* msg);
};

#endif /* HP_COMPONENT_H_ */

