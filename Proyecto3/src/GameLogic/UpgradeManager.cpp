#include "UpgradeManager.h"
#include "LogSystem.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"
#include "GameObject.h"
#include "Pool.h"
#include "UpgradeComponent.h"
#include "BulletHittable.h"

UpgradeManager::UpgradeManager()
{
	upgrades = new nap_Pool("pref_upgrade");
	weaponUpgrades = vector<int>{ 2, 3 };
}

void UpgradeManager::setUp(GameObject * player)
{
	player_ = player;
}

void UpgradeManager::generateUpgrades(int round)
{
	GameObject* o = upgrades->getItem();
	static_cast<UpgradeComponent*>(o->getComponent("upgrade_comp"))->setUpgrade(0);
	o->setPosition(nap_vector3(0, 4, -8));

	GameObject* p = upgrades->getItem();
	int rnd = rand() % weaponUpgrades.size();
	static_cast<UpgradeComponent*>(p->getComponent("upgrade_comp"))->setUpgrade(weaponUpgrades[rnd]);
	p->setPosition(nap_vector3(0, 4, 0));

	if (round > 3 && weaponUpgrades.size() > 1) {
		GameObject* c = upgrades->getItem();
		int rnd2 = rand() % weaponUpgrades.size();
		while (rnd == rnd2)
			rnd2 = rand() % weaponUpgrades.size();
		static_cast<UpgradeComponent*>(c->getComponent("upgrade_comp"))->setUpgrade(rnd2);
		c->setPosition(nap_vector3(0, 4, 8));
	}
}

void UpgradeManager::destroyUpgrades()
{
	upgrades->resetPool();
}

void UpgradeManager::activeUpgrade(int upgrade)
{
	switch (upgrade)
	{
	case 0:
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(ADD_HP), player_);
		break;
	case 1:
		MessageSystem::getSingleton()->sendMessageGameObject(&Message(ADD_SPEED), player_);
		break;
	case 2:
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_UPGRADE_TAKEN(upgrade), player_);
		removeUpgrade(upgrade);
		weaponUpgrades.push_back(4);
		weaponUpgrades.push_back(5);
		weaponUpgrades.push_back(8);
		break;
	case 3:
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_UPGRADE_TAKEN(upgrade), player_);
		removeUpgrade(upgrade);
		weaponUpgrades.push_back(6);
		weaponUpgrades.push_back(7);
		weaponUpgrades.push_back(9);
		weaponUpgrades.push_back(10);
		break;
	case 4:
		BulletHittable::updateBulletDamage("bBullets", 3);
		break;
	case 5:
		BulletHittable::updateBulletDamage("pBullets", 6);
		break;
	case 6:
		BulletHittable::updateBulletDamage("dBullets", 2);
		break;
	case 7:
	case 8:
	case 9:
	case 10:
		MessageSystem::getSingleton()->sendMessageGameObject(&Msg_UPGRADE_TAKEN(upgrade), player_);
		break;
	default:
		break;
	}
}

void UpgradeManager::removeUpgrade(int upgrade)
{
	int i = 0;
	while (i < weaponUpgrades.size() && weaponUpgrades[i] != upgrade)
		i++;
	if (i < weaponUpgrades.size())
		weaponUpgrades.erase(weaponUpgrades.begin() + i);
}
