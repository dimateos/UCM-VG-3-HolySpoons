//Nap_Time_Studios
#ifndef BULLETSHOOTER_H_
#define BULLETSHOOTER_H_

//fowarding
class nap_Pool;
class Weapon;
#include <map>
#include <vector>
#include "Component.h"

//component to set live and etc to stuff impacted by bullets
class BulletShooter : public Component
{
public:
	inline BulletShooter(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	virtual ~BulletShooter();
	virtual void setUp();

	//atm just left click to shoot
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void update(GameObject* o, double time);
	virtual void receive(Message* msg);

	static std::map<string, int> bulletDamages;
	static std::vector<int> bulletUpgrades;
	static void updateBulletDamage(string name, int damage);

protected:
	virtual void changeWeapon(int n);
	virtual void activeWeapon(int n, bool active);
	virtual void addWeapon(string prefab, string weaponType, float vel, double shootSpeed);

	nap_transform* owner_trans_;
	float relY_, relZ_;

	SDL_Keycode first_, second_, third_;

	bool sprinting = false;
	int currentWeapon = 0;
	vector<Weapon*> weapons;
};

#endif /* BULLETSHOOTER_H_ */
