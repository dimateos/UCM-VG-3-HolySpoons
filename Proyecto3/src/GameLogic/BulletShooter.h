//Nap_Time_Studios
#ifndef BULLETSHOOTER_H_
#define BULLETSHOOTER_H_

class nap_Pool; //fowarding

#include "Component.h"
class BulletShooter : public Component
{
public:
	inline BulletShooter(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~BulletShooter() {};
	virtual void setUp();

	//atm just left click to shoot
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);

protected:
	nap_transform* owner_trans_;
	float relY_, relZ_, vel_; //vel maybe bullet prefabs and depending on that

	nap_Pool* pool_ = nullptr;
	virtual void shoot();
};

#endif /* BULLETSHOOTER_H_ */
