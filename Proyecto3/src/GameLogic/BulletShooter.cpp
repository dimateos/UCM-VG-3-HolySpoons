#include "BulletShooter.h"
#include "PhysicsComponent.h"
#include "Pool.h"
#include "GOFactory.h"

void BulletShooter::setUp() {
	owner_trans_ = owner_->getTransPtr();
	relY_ = cfg_["relativeY"];
	relZ_ = cfg_["relativeZ"];
	vel_ = cfg_["vel"];

	pool_ = new nap_Pool(cfg_["itemString"]);
	pool_->setDefault(0);
	pool_->init();
}

bool BulletShooter::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	switch (evt.type) {

	case SDL_MOUSEBUTTONUP:
		if (evt.button.button == SDL_BUTTON_LEFT) {
			shoot();
			handled = true;
		}
		break;
	}

	return handled;
}

void BulletShooter::shoot() {
	//add to state
	GameObject* bul = pool_->getItem();
	bul->setActive();

	//dir
	nap_vector3 dir = owner_trans_->q_.toNapVec3(vZ*-1);

	//vel (uses dir y)
	static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * vel_).px());

	//vel (ignores dir y)
	dir.y_ = 0;
	dir.normalize();
	bul->setPosition(owner_trans_->p_ + vY * relY_ + dir * relZ_);
}

REGISTER_TYPE(BulletShooter);
