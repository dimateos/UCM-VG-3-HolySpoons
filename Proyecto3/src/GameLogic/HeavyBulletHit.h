//Nap_Time_Studios
#ifndef HEAVYBULLETHIT_H_
#define HEAVYBULLETHIT_H_

#include "BulletHit.h"

//component to deactive bullets
class HeavyBulletHit : public BulletHit
{
public:
	inline HeavyBulletHit(nap_json const & cfg, GameObject* owner) : BulletHit(cfg, owner) {};
	inline virtual ~HeavyBulletHit() {};

	virtual void onCollision(ID* other);
};

#endif /* HEAVYBULLETHIT_H_ */
