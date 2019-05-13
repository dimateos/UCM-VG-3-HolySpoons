//Nap_Time_Studios
#ifndef UPGRADEMANAGER_H_
#define UPGRADEMANAGER_H_
#include <vector>

class nap_Pool;
class GameObject;


class UpgradeManager
{
public:
	UpgradeManager();
	void setUp(GameObject* player);
	virtual ~UpgradeManager() { delete upgrades; };
	void generateUpgrades(int round);
	void destroyUpgrades();
	void activeUpgrade(int upgrade);
protected:
	GameObject* player_ = nullptr;
	nap_Pool* upgrades;
	//vector<int> upgradeCount;

	void removeUpgrade(int upgrade);

	std::vector<int> weaponUpgrades;
	//vector<int> bodyUpgrades;
};

#endif /* UPGRADEMANAGER_H_ */
