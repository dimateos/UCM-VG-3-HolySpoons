#include "BulletShooter.h"
#include <GlobalConfig.h>

//fowarded
#include "Pool.h"
#include "Weapon.h"
#include "Messages.h"
#include "RenderComponent.h"

#include <SDL_events.h>	//events

std::map<string, int> BulletShooter::bulletDamages{};
std::vector<int> BulletShooter::bulletUpgrades{};

void BulletShooter::updateBulletDamage(string name, int damage)
{
	auto it = bulletDamages.find(name);
	if (it != bulletDamages.end())
		it->second += damage;
}

BulletShooter::~BulletShooter() {
	for (Weapon* w : weapons)
		delete w;
}

void BulletShooter::setUp() {
	owner_trans_ = owner_->getTransPtr();
	relY_ = cfg_["relativeY"];
	relZ_ = cfg_["relativeZ"];

	bulletDamages = std::map<string, int>{ { "bBullets", FINDnRETURN(cfg_, "bDamage", int, 12) } ,
		{ "pBullets", FINDnRETURN(cfg_, "pDamage", int, 30) },
		{ "dBullets", FINDnRETURN(cfg_, "dDamage", int, 10) } };

	bulletUpgrades = std::vector<int>{ FINDnRETURN(cfg_, "bUpgrade", int, 3),
		FINDnRETURN(cfg_, "pUpgrade", int, 6),
		FINDnRETURN(cfg_, "dUpgrade", int, 2) };

	

	addWeapon("base_bullet", "baseSpoon", FINDnRETURN(cfg_, "bSpeed", float, 110), FINDnRETURN(cfg_, "bFireRate", double, 0.2),
		owner_->getComponent("baseWeaponSound"));
	addWeapon("power_bullet", "powerSpoon", FINDnRETURN(cfg_, "pSpeed", float, 90), FINDnRETURN(cfg_, "pFireRate", double, 0.6), 
		owner_->getComponent("powerWeaponSound"));
	addWeapon("disperse_bullet", "shotSpoon", FINDnRETURN(cfg_, "dSpeed", float, 110), FINDnRETURN(cfg_, "dFireRate", double, 0.8), 
		owner_->getComponent("spreadWeaponSound"));
	currentWeapon = 0;

	//read the keys from global cfg
	first_ = GlobalCFG::keys["FIRST"];
	second_ = GlobalCFG::keys["SECOND"];
	third_ = GlobalCFG::keys["THIRD"];

	renderCompName = FINDnRETURN_s(cfg_, "renderCompName", "spoon_ren");
}

void BulletShooter::lateSetUp()
{
	// render component of the weapon of the player
	renderComp = static_cast<RenderComponent*>(owner_->getComponent(renderCompName));
}

bool BulletShooter::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	switch (evt.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (sprinting) return handled;
		if (evt.button.button == SDL_BUTTON_LEFT) {
			weapons[currentWeapon]->mouseUpdate(true);
			handled = true;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (sprinting) return handled;
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

	if (msg->id_ == SPRINT_ON) {
		weapons[currentWeapon]->mouseUpdate(false);
		sprinting = true;
	}
	else if (msg->id_ == SPRINT_OFF) {
		sprinting = false;
	}
	else if (msg->id_ == STATE_IN) {
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
		case 4:
			updateBulletDamage("bBullets", bulletUpgrades[0]);
			break;
		case 5:
			updateBulletDamage("pBullets", bulletUpgrades[1]);
			break;
		case 6:
			updateBulletDamage("dBullets", bulletUpgrades[2]);
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
		renderComp->setMaterial(weapons[n]->getMaterial());
	}
}

void BulletShooter::activeWeapon(int n, bool active = true) {
	if (n >= 0 && n < weapons.size()) {
		weapons[n]->setActive(active);
		changeWeapon(n);
	}
}

void BulletShooter::addWeapon(string prefab, string weaponType, float vel, double shootSpeed, Component* soundComponent) {
	if (weaponType == "baseSpoon")
		weapons.push_back(new BaseSpoon(prefab, vel, shootSpeed, soundComponent, "Spoon"));
	else if (weaponType == "powerSpoon")
		weapons.push_back(new PowerSpoon(prefab, vel, shootSpeed, soundComponent, "MetalSpoon"));
	else if (weaponType == "shotSpoon")
		weapons.push_back(new ShotSpoon(prefab, vel, shootSpeed, soundComponent, "GoldenSpoon"));
}

#include "GOFactory.h"
REGISTER_TYPE(BulletShooter);
