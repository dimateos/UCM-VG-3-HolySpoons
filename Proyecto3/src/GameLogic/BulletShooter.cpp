#include "BulletShooter.h"
#include <GlobalConfig.h>

//fowarded
#include "Pool.h"
#include "Weapon.h"
#include "Messages.h"

#include <SDL_events.h>	//events

BulletShooter::~BulletShooter() {
	for (Weapon* w : weapons)
		delete w;
}

void BulletShooter::setUp() {
	owner_trans_ = owner_->getTransPtr();
	relY_ = cfg_["relativeY"];
	relZ_ = cfg_["relativeZ"];

	//later cfged in json
	addWeapon("base_bullet", "baseSpoon", 110, 0.2);
	addWeapon("power_bullet", "powerSpoon", 90, 0.6);
	addWeapon("disperse_bullet", "shotSpoon", 110, 0.8);
	currentWeapon = 0;

	//read the keys from global cfg
	first_ = GlobalCFG::keys["FIRST"];
	second_ = GlobalCFG::keys["SECOND"];
	third_ = GlobalCFG::keys["THIRD"];
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
		{
			handled = true;
			if (evt.key.keysym.sym == first_) changeWeapon(0);
			if (evt.key.keysym.sym == second_) changeWeapon(1);
			if (evt.key.keysym.sym == third_) changeWeapon(2);
			else handled = false;
			break;
		}
	}

	return handled;
}

void BulletShooter::update(GameObject * o, double time) {
	weapons[currentWeapon]->shootUpdate(owner_trans_, relY_, relZ_, time);
}

void BulletShooter::receive(Message * msg) {

	if (msg->id_ == STATE_IN) {
		weapons[currentWeapon]->mouseUpdate(false);
	}
	else if (msg->id_ == UPGRADE_TAKEN) {
		int upgrade = static_cast<Msg_UPGRADE_TAKEN*>(msg)->upgrade_;
		switch (upgrade)
		{
		case 2:
		case 3:
			activeWeapon(upgrade - 1, true);
			break;
		case 7:
		case 8:
		case 9:
			weapons[upgrade - 7]->shootSpeed_ -= weapons[upgrade - 7]->shootSpeed_ / 12;
			break;
		case 10:
			static_cast<ShotSpoon*>(weapons[2])->nBullets += 1;
			break;
		default:
			break;
		}
	}
}

void BulletShooter::changeWeapon(int n) {
	if (n >= 0 && n < weapons.size() && weapons[n]->isActive()) {
		currentWeapon = n;
		weapons[n]->swapDelay();
	}
}

void BulletShooter::activeWeapon(int n, bool active = true) {
	if (n >= 0 && n < weapons.size()) {
		weapons[n]->setActive(active);
		changeWeapon(n);
	}
}

void BulletShooter::addWeapon(string prefab, string weaponType, float vel, double shootSpeed) {
	if (weaponType == "baseSpoon")
		weapons.push_back(new BaseSpoon(prefab, vel, shootSpeed));
	else if (weaponType == "powerSpoon")
		weapons.push_back(new PowerSpoon(prefab, vel, shootSpeed));
	else if (weaponType == "shotSpoon")
		weapons.push_back(new ShotSpoon(prefab, vel, shootSpeed));
}

#include "GOFactory.h"
REGISTER_TYPE(BulletShooter);
