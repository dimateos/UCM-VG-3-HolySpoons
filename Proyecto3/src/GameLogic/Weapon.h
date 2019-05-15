//Nap_Time_Studios
#ifndef WEAPON_H_
#define WEAPON_H_

#include <string>
#include <Timer.h>
using namespace std;

//fowarding
class nap_transform;
class nap_Pool;

#include "Activable.h"
//base weapon class
class Weapon : public Activable
{
public:
	Weapon(string prefab, string material, float vel, double shootSpeed);
	virtual ~Weapon();

	virtual void mouseUpdate(bool down);
	virtual void shootUpdate(nap_transform* owner_trans,
		float relY, float relZ, double time);
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
	virtual void swapDelay();
	double shootSpeed_;
	inline string getMaterial() const { return material_; }
protected:
	bool down_ = false, ready_ = true;
	nap_Pool* pool_ = nullptr;
	float vel_ = 0;
	napTimer t;
	string material_;
};

//automatic shooting
class BaseSpoon : public Weapon
{
public:
	BaseSpoon(string prefab, string material, float vel, double shootSpeed);
	virtual ~BaseSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
protected:
	int spread = 2;
};

//powerfull semi-automatic shot
class PowerSpoon : public Weapon
{
public:
	PowerSpoon(string prefab, string material, float vel, double shootSpeed);
	virtual ~PowerSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
};

//powerfull automatic shooting (multiple bullets with spread)
class ShotSpoon : public Weapon
{
public:
	ShotSpoon(string prefab, string material, float vel, double shootSpeed);
	virtual ~ShotSpoon();
	virtual void shoot(nap_transform* owner_trans,
		float relY, float relZ);
	int nBullets = 5;
protected:
	int maxSpreadX = 8, maxSpreadY = 8;
	float iniDispersionFactor = 1/40;
};

#endif /* WEAPON_H_ */