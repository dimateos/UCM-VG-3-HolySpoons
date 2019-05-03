#include "SniperBehavior.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>
#include "GOFactory.h"
#include "PhysicsComponent.h"
#include "PxRigidDynamic.h"

#include <Transforms.h>

#define defInitialBullets 10
void SniperBehavior::advanceBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;

	nap_vector3 ownerPos = owner_->getPosition();

	ownerPos.y_ += time * speed;


	if (ownerPos.y_ >= highY) {
		t.start();
		status = attack;
	}
	owner_->setPosition(ownerPos);

}

void SniperBehavior::attackBehavior(float time)
{
	if (t.update(time)) {
		shoot();
		t.start();
		status = back;
	}
}

void SniperBehavior::backBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;


	nap_vector3 ownerPos = owner_->getPosition();

	ownerPos.y_ -= time * speed;


	if (ownerPos.y_ < lowY) {
		t.start();
		status = advance;
		ownerPos = positions[rand() % positions.size()];
		ownerPos.y_ += lowY;
		float tmp = rand() % 100 - 50;
		if (ownerPos.x_ == 0)
			ownerPos.x_ = tmp;
		else
			ownerPos.z_ = tmp;

	}
	owner_->setPosition(ownerPos);
}

void SniperBehavior::shoot()
{
	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = { dest->p_.x_ - ownerPos.x_, dest->p_.y_ - ownerPos.y_, dest->p_.z_ - ownerPos.z_ }; //simple vector calc dest - src
	dir = dir.normalize();
	GameObject* o = p->getItem();
	o->setPosition(owner_->getPosition() + dir*6);
	static_cast<PhysicsComponent*>(o->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * shotSpeed).px());
}

void SniperBehavior::configActive()
{
	status = advance;
	t.start();
	owner_->setPosition(positions[rand() % positions.size()] + nap_vector3(0, lowY, 0));
}

void SniperBehavior::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	speed = cfg_["speed"];
	if (FIND(cfg_, "shotSpeed")) shotSpeed = this->cfg_["shotSpeed"];
	if (FIND(cfg_, "actionDelay")) t.start(this->cfg_["actionDelay"]);
	if (FIND(cfg_, "lowY")) lowY = this->cfg_["lowY"];
	if (FIND(cfg_, "highY")) highY = this->cfg_["highY"];

	p = new nap_Pool("e_bullet");
	p->setDefault(defInitialBullets);
	p->init();
	positions.push_back(nap_vector3(85, 0, 0));
	positions.push_back(nap_vector3(0, 0, 85));
	positions.push_back(nap_vector3(-85, 0, 0));
	positions.push_back(nap_vector3(0, 0, -85));

	string target = this->cfg_["Target"];
	GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject(target);

	if (o != nullptr) {
		dest = o->getTransPtr();
	}
	else {
		LogSystem::Log("GameObject " + target + " does not exist in the current state", LogSystem::GAME,  {__FILE__, __LINE__} );
	}
	owner_->setPosition(positions[rand() % positions.size()] + nap_vector3(0,lowY,0));
}

void SniperBehavior::update(GameObject* ent, double time)
{
	switch (status)
	{
	case advance:
		advanceBehavior(time);
		break;
	case attack:
		attackBehavior(time);
		break;
	case back:
		backBehavior(time);
		break;
	default:
		break;
	}
}

REGISTER_TYPE(SniperBehavior);
