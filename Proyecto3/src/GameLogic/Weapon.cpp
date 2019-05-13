#include "Weapon.h"

//fowarded
#include "Pool.h"
#include "GameObject.h"
#include "PhysicsComponent.h"
#include "PxRigidDynamic.h"

#include <Transforms.h>

#define defInitialBullets 10
Weapon::Weapon(string prefab, float vel = 30, double shootSpeed = 0.2) {
	active_ = false;
	vel_ = vel;
	shootSpeed_ = shootSpeed;
	t.start(shootSpeed);
	pool_ = new nap_Pool(prefab);
	pool_->setDefault(defInitialBullets);
	pool_->init();
}

Weapon::~Weapon() {
	delete pool_;
}

void Weapon::mouseUpdate(bool down) {
	down_ = down;
}

void Weapon::shootUpdate(nap_transform * owner_trans, float relY, float relZ, double time) {
	if (t.update(time))
		ready_ = true;
	if (ready_ && down_) {
		ready_ = false;
		t.start(shootSpeed_);
		shoot(owner_trans, relY, relZ);
	}
}

void Weapon::shoot(nap_transform* owner_trans, float relY, float relZ) {
	//add to state
	GameObject* bul = pool_->getItem();

	//dir
	nap_vector3 dir = owner_trans->q_.toNapVec3(vZ*-1);

	//vel (uses dir y)
	static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * vel_).px());

	//pos (moves relative)
	bul->setPosition(owner_trans->p_ + vY * relY + dir * relZ);
}

void Weapon::swapDelay() {
	t.start(shootSpeed_);
}

////////////////////////////////////////////////////////////////////////////////

BaseSpoon::BaseSpoon(string prefab, float vel, double shootSpeed) : Weapon(prefab, vel, shootSpeed) {
	active_ = true;
}

BaseSpoon::~BaseSpoon() {}

void BaseSpoon::shoot(nap_transform* owner_trans, float relY, float relZ) {
	Weapon::shoot(owner_trans, relY, relZ);
}

////////////////////////////////////////////////////////////////////////////////

PowerSpoon::PowerSpoon(string prefab, float vel, double shootSpeed) : Weapon(prefab, vel, shootSpeed) {
	//active_ = true;
}

PowerSpoon::~PowerSpoon() {}

void PowerSpoon::shoot(nap_transform * owner_trans, float relY, float relZ) {
	down_ = false;
	Weapon::shoot(owner_trans, relY, relZ);
}

////////////////////////////////////////////////////////////////////////////////

ShotSpoon::ShotSpoon(string prefab, float vel, double shootSpeed) :Weapon(prefab, vel, shootSpeed) {
	//active_ = true;
}

ShotSpoon::~ShotSpoon() {}

#define pi 3.141592
#define toRadian (pi / 180)
void ShotSpoon::shoot(nap_transform * owner_trans, float relY, float relZ) {
	down_ = false;

	//get the owner axes
	nap_vector3 dirZ = owner_trans->q_.toNapVec3(vZ*-1);
	nap_vector3 dirX = owner_trans->q_.toNapVec3(vX*-1);
	nap_vector3 dirY = owner_trans->q_.toNapVec3(vY*-1);

	//add to state
	GameObject* bul = pool_->getItem();
	nap_vector3 tmpDir;
	PxQuat qx, qy;

	float rndX, rndY;

	for (size_t i = 0; i < nBullets; i++) {
		rndX = rand() % (2*maxSpreadX) - maxSpreadX;
		rndY = rand() % (2 * maxSpreadY) - maxSpreadY;
		//add to state
		bul = pool_->getItem();

		//rotate dir using its axis instead of global
		qx = PxQuat(rndX * toRadian, dirX.px());
		qy = PxQuat(rndY * toRadian, dirY.px());
		tmpDir = napVEC3((qx*qy).rotate(dirZ.px()));

		//vel
		static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((tmpDir * vel_).px());
		//pos (moves relative)
		bul->setPosition(owner_trans->p_ + vY * relY + dirZ * relZ + dirY * (rndX*iniDispersionFactor) + dirX * (rndY*iniDispersionFactor));
	}
}
