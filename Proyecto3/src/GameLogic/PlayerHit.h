//Nap_Time_Studios
#ifndef PLAYERTHIT_H_
#define PLAYERTHIT_H_

#include <Timer.h> //invunerability

#include <string>
#include <list>
struct damageDeal
{
	inline damageDeal(std::string _group, int _dmg) : group(_group), dmg(_dmg) {};
	std::string group;
	int dmg;
};

#include "CollisionListener.h"
#include "Component.h"
class PlayerHit : public Component, public CollisionListener
{
public:
	inline PlayerHit(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {};
	inline virtual ~PlayerHit() {};
	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual void onCollision(ID* other);

protected:
	virtual void configActive();

	napTimer inv_t;
	bool inv = false;
	std::list<damageDeal> damageDealers_;
};

#endif /* PLAYERTHIT_H_ */
