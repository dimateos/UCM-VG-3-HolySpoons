//Nap_Time_Studios
#ifndef WEAPON_H_
#define WEAPON_H_

#include "Pool.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "PxRigidDynamic.h"
#include "Activable.h"
class Weapon: public Activable
{
public:
	Weapon(string prefab, float vel, double shootSpeed);
	 ~Weapon();
	virtual void mouseUpdate(bool down);
	virtual void shootUpdate(nap_transform* owner_trans,
		float relY, float relZ, double time);
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
	virtual void swapDelay();
protected:
	bool down_ = false;
	nap_Pool* pool_ = nullptr;
	float vel_ = 0;
	double shootSpeed_ = 0.2, timer = 0;
};

class BaseSpoon : public Weapon
{
public:
	BaseSpoon(string prefab, float vel, double shootSpeed);
	 ~BaseSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
};

class PowerSpoon : public Weapon
{
public:
	PowerSpoon(string prefab, float vel, double shootSpeed);
	 ~PowerSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
};

class ShotSpoon : public Weapon
{
public:
	ShotSpoon(string prefab, float vel, double shootSpeed);
	~ShotSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
};

#endif /* WEAPON_H_ */