#include "UpgradeManager.h"
#include "LogSystem.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"
#include "GameObject.h"
#include "Pool.h"
#include "UpgradeComponent.h"
#include "RenderComponent.h"
#include "BulletHittable.h"


UpgradeManager::UpgradeManager()
{
	upgrades = new nap_Pool("pref_upgrade");
	weaponUpgrades = vector<int>{ 2, 3 };
	upgradeMaterials = vector<string>{ "Health", "Speed", "aGSpoon", "aMSpoon", "dWSpoon", "dGSpoon", "dMSpoon", "fWSpoon", "fGSpoon", "fMSPoon", "MSpoon" };
}

void UpgradeManager::setUp(GameObject * player)
{
	player_ = player;
	basePos = player->getPosition();
}

void UpgradeManager::generateUpgrades(int round)
{
	GameObject* o = upgrades->getItem();
	int upg = rand() % 2;
	static_cast<UpgradeComponent*>(o->getComponent("upgrade_comp"))->setUpgrade(upg);
	static_cast<RenderComponent*>(o->getComponent("up_render"))->setMaterial(upgradeMaterials[upg]);
	o->setPosition(basePos + nap_vector3(0, 1, -8));

	GameObject* p = upgrades->getItem();
	int rnd = weaponUpgrades[rand() % weaponUpgrades.size()];
	static_cast<UpgradeComponent*>(p->getComponent("upgrade_comp"))->setUpgrade(rnd);
	static_cast<RenderComponent*>(p->getComponent("up_render"))->setMaterial(upgradeMaterials[rnd]);
	p->setPosition(basePos + nap_vector3(0, 1, 0));

	if (round > 3 && weaponUpgrades.size() > 1) {
		GameObject* c = upgrades->getItem();
		int rnd2 = weaponUpgrades[rand() % weaponUpgrades.size()];
		while (rnd == rnd2)
			rnd2 = weaponUpgrades[rand() % weaponUpgrades.size()];
		static_cast<UpgradeComponent*>(c->getComponent("upgrade_comp"))->setUpgrade(rnd2);
		static_cast<RenderComponent*>(c->getComponent("up_render"))->setMaterial(upgradeMaterials[rnd2]);
		c->setPosition(basePos + nap_vector3(0, 1, 8));
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
	case 5:
	case 6:
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
