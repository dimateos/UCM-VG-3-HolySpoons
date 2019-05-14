//Nap_Time_Studios
#ifndef UPGRADEMANAGER_H_
#define UPGRADEMANAGER_H_
#include <vector>
//#include <map>
#include <Transforms.h>
//#include <string>

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
	nap_vector3 basePos;
	//vector<int> upgradeCount;

	void removeUpgrade(int upgrade);

	std::vector<int> weaponUpgrades;
	//vector<int> bodyUpgrades;
	std::vector<std::string> upgradeMaterials;
	//std::map<int, string> upgradeMaterials;
};

#endif /* UPGRADEMANAGER_H_ */
