//Nap_Time_Studios
#ifndef NAP_POOL_H_
#define NAP_POOL_H_

#include <vector>
class GameObject;

class nap_Pool {
private:
	std::vector<GameObject*> pool;
	int defaultValue = 10;
	GameObject* spawn();
	GameObject* original = nullptr;
	std::string entityName;

public:
	nap_Pool(std::string entity);
	virtual ~nap_Pool();

	GameObject* getItem();
	void init();

	std::vector<GameObject*> getPool() { return pool; };
	inline void setDefault(int def) { defaultValue = def; };
};

#endif /* NAP_POOL_H_ */

