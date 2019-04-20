#include "Weapon.h"



Weapon::Weapon(string prefab, float vel = 30, double shootSpeed = 0.2)
{
	active_ = false;
	vel_ = vel;
	shootSpeed_ = shootSpeed;
	pool_ = new nap_Pool(prefab);
	pool_->setDefault(0);
	pool_->init();
}


Weapon::~Weapon()
{
	delete pool_;
}

void Weapon::mouseUpdate(bool down)
{
	down_ = down;
}

void Weapon::shootUpdate(nap_transform * owner_trans, float relY, float relZ, double time)
{
	timer -= time;
	if (timer <= 0 && down_) {
		timer = shootSpeed_;
		shoot(owner_trans, relY, relZ);
	}
}

void Weapon::shoot(nap_transform* owner_trans,
	float relY, float relZ)
{
}

void Weapon::swapDelay()
{
	timer = 0.5;
}

BaseSpoon::BaseSpoon(string prefab, float vel, double shootSpeed) : Weapon(prefab, vel, shootSpeed)
{
	active_ = true;
}

BaseSpoon::~BaseSpoon()
{
}

void BaseSpoon::shoot(nap_transform* owner_trans,
	float relY, float relZ)
{
	//add to state
	GameObject* bul = pool_->getItem();
	bul->setActive();

	//dir
	nap_vector3 dir = owner_trans->q_.toNapVec3(vZ*-1);

	//vel (uses dir y)
	static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * vel_).px());

	//vel (ignores dir y)
	dir.y_ = 0;
	dir.normalize();
	bul->setPosition(owner_trans->p_ + vY * relY + dir * relZ);
}

PowerSpoon::PowerSpoon(string prefab, float vel, double shootSpeed) :Weapon(prefab, vel, shootSpeed)
{
	active_ = true;
}

PowerSpoon::~PowerSpoon()
{
}

void PowerSpoon::shoot(nap_transform * owner_trans, float relY, float relZ)
{
	down_ = false;

	//add to state
	GameObject* bul = pool_->getItem();
	bul->setActive();
	

	//dir
	nap_vector3 dir = owner_trans->q_.toNapVec3(vZ*-1);

	//vel (uses dir y)
	static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * vel_).px());

	//vel (ignores dir y)
	dir.y_ = 0;
	dir.normalize();
	bul->setPosition(owner_trans->p_ + vY * relY + dir * relZ);
}

ShotSpoon::ShotSpoon(string prefab, float vel, double shootSpeed) :Weapon(prefab, vel, shootSpeed)
{
	active_ = true;
}

ShotSpoon::~ShotSpoon()
{
}

void ShotSpoon::shoot(nap_transform * owner_trans, float relY, float relZ)
{
	for (size_t i = 0; i < 3; i++)
	{
		//add to state
		GameObject* bul = pool_->getItem();
		bul->setActive();


		//dir
		nap_vector3 dir = owner_trans->q_.toNapVec3(vZ*-1);


		//vel (uses dir y)
		static_cast<PhysicsComponent*>(bul->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * vel_).px());

		//vel (ignores dir y)
		dir.y_ = 0;
		dir.normalize();
		bul->setPosition(owner_trans->p_ + vY * relY + dir * relZ);
	}
	
}
