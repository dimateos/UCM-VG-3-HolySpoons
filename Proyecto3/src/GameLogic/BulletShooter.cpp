#include "BulletShooter.h"

#include "Pool.h"
#include "PhysicsComponent.h"
#include <PxRigidDynamic.h>

BulletShooter::~BulletShooter()
{
	for (Weapon* w: weapons)
		delete w;
}

void BulletShooter::setUp() {
	owner_trans_ = owner_->getTransPtr();
	relY_ = cfg_["relativeY"];
	relZ_ = cfg_["relativeZ"];

	addWeapon("base_bullet","baseSpoon", 30,0.2);
	addWeapon("power_bullet", "powerSpoon", 15, 0.4);
	addWeapon("base_bullet", "shotSpoon", 30, 0.3);
	currentWeapon = 0;
}

bool BulletShooter::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	switch (evt.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (evt.button.button == SDL_BUTTON_LEFT) {
			weapons[currentWeapon]->mouseUpdate(true);
			handled = true;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (evt.button.button == SDL_BUTTON_LEFT) {
			weapons[currentWeapon]->mouseUpdate(false);
			handled = true;
		}
		break;
	case SDL_KEYUP:
		int tmp = (int)(evt.key.keysym.sym - SDLK_1);
		if (tmp >= 0 && tmp < 10) {
			changeWeapon(tmp);
			handled = true;
		}
		break;
	}

	return handled;
}

void BulletShooter::update(GameObject * o, double time)
{
	weapons[currentWeapon]->shootUpdate(owner_trans_, relY_, relZ_, time);
}

void BulletShooter::changeWeapon(int n)
{
	if (n >= 0 && n < weapons.size() && weapons[currentWeapon]->isActive()) {
		currentWeapon = n;
		weapons[currentWeapon]->swapDelay();
	}
}

void BulletShooter::activeWeapon(int n, bool active = true)
{
	if (n >= 0 && n < weapons.size()) {
		weapons[currentWeapon]->setActive(active);
		changeWeapon(n);
	}
}

void BulletShooter::addWeapon(string prefab, string weaponType, float vel, double shootSpeed)
{
	if (weaponType == "baseSpoon")
		weapons.push_back(new BaseSpoon(prefab, vel, shootSpeed));
	else if(weaponType == "powerSpoon")
		weapons.push_back(new PowerSpoon(prefab, vel, shootSpeed));
	else if (weaponType == "shotSpoon")
		weapons.push_back(new ShotSpoon(prefab, vel, shootSpeed));

}

#include "GOFactory.h"
REGISTER_TYPE(BulletShooter);
