//Nap_Time_Studios
#ifndef BULLETSHOOTER_H_
#define BULLETSHOOTER_H_

class nap_Pool; //fowarding

#include "Component.h"
#include "Weapon.h"
class BulletShooter : public Component
{
public:
	inline BulletShooter() {}
	inline BulletShooter(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~BulletShooter();
	virtual void setUp();

	//atm just left click to shoot
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void update(GameObject* o, double time);

protected:
	virtual void changeWeapon(int n);
	virtual void activeWeapon(int n, bool active);
	virtual void addWeapon(string prefab, string weaponType, float vel, double shootSpeed);

	nap_transform* owner_trans_;
	float relY_, relZ_;

	
	int currentWeapon = 0;
	vector<Weapon*> weapons;
};

#endif /* BULLETSHOOTER_H_ */
