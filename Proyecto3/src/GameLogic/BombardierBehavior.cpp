#include "BombardierBehavior.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>
#include "GOFactory.h"

#include <Transforms.h>

#define defInitialBullets 10
void BombardierBehavior::advanceBehavior(float time)
{
	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = { dest->p_.x_ - ownerPos.x_, lowY - ownerPos.y_, dest->p_.z_ - ownerPos.z_ }; //simple vector calc dest - src (we dont want to follow the y coord)
	//if (dir.y_ > 0) dir.y_ = 0;
	dir = dir.normalize();

	//move towards dest
	ownerPos.x_ += dir.x_*time*speed;
	ownerPos.y_ += dir.y_*time*speed;
	ownerPos.z_ += dir.z_*time*speed;

	owner_->setPosition(ownerPos);
	if (abs(dest->p_.x_ - ownerPos.x_) + abs(lowY - ownerPos.y_) + abs(dest->p_.z_ - ownerPos.z_) < 4) {
		status = attack;
		orig.x_ = ownerPos.x_ + rand() % 200 - 100;
		orig.z_ = ownerPos.z_ + rand() % 200 - 100;
	}
}

void BombardierBehavior::attackBehavior(float time)
{
	if (t.update(time)) {
		shoot();
		t.start();
		currentCharge--;
		if (currentCharge == 0) {
			status = back;
			currentCharge = charge;
		}
	}
}

void BombardierBehavior::backBehavior(float time)
{
	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = {orig.x_ - ownerPos.x_, highY - ownerPos.y_, orig.z_ - ownerPos.z_ }; //simple vector calc orig - src (we dont want to follow the y coord)
	//if (dir.y_ < 0) dir.y_ = 0;
	dir = dir.normalize();

	//move towards orig
	ownerPos.x_ += dir.x_*time*speed;
	ownerPos.y_ += dir.y_*time*speed;
	ownerPos.z_ += dir.z_*time*speed;

	owner_->setPosition(ownerPos);
	if (abs(orig.x_ - ownerPos.x_) + abs(highY - ownerPos.y_) + abs(orig.z_ - ownerPos.z_) < 4)
		status = advance;
}

void BombardierBehavior::shoot()
{
	GameObject* o = p->getItem();
	o->setPosition(owner_->getPosition() - (vY * 3) + (vZ * (rand() % 6 - 3)) + (vX * (rand() % 6 - 3)));
}

void BombardierBehavior::configActive()
{
	status = advance;
	currentCharge = charge;
}

void BombardierBehavior::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	speed = cfg_["speed"];
	if (FIND(cfg_, "highY")) highY = this->cfg_["highY"];
	if (FIND(cfg_, "lowY")) lowY = this->cfg_["lowY"];
	if (FIND(cfg_, "shootSpeed")) t.start(this->cfg_["shootSpeed"]);
	if (FIND(cfg_, "chargeSize")) charge = this->cfg_["chargeSize"];
	p = new nap_Pool("e_bomb");
	p->setDefault(defInitialBullets);
	p->init();

	string target = this->cfg_["Target"];
	GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject(target);

	if (o != nullptr) {
		dest = o->getTransPtr();
	}
	else {
		LogSystem::Log("GameObject " + target + " does not exist in the current state", LogSystem::GAME,  {__FILE__, __LINE__} );
	}
}

void BombardierBehavior::update(GameObject* ent, double time)
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

REGISTER_TYPE(BombardierBehavior);
