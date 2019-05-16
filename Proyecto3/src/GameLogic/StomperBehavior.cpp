#include "StomperBehavior.h"
#include "GameStateMachine.h"
#include "LogSystem.h"
#include <math.h>
#include "GOFactory.h"
#include "PhysicsComponent.h"
#include "PxRigidDynamic.h"

#include "MessageSystem.h"
#include <Transforms.h>

#define defInitialBullets 10
void StomperBehavior::moveBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;

	owner_->setPosition(positions[rand() % positions.size()] + nap_vector3(0, lowY, 0));
	MessageSystem::getSingleton()->sendMessageGameObject(&Message(PLAY_SOUND), owner_);
	i++;

	if (i < maxJumps)
		t.start(0.6);
	else {
		i = 0;
		lastAttack++;
		if (lastAttack > 3)
			lastAttack = 1;
		status = (e_state)lastAttack;
		t.start(actionDelay);
	}
}

void StomperBehavior::jumpBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;

	float tmp = time * speed;
	if (!up) {
		tmp = -tmp;
		if (baseY <= lowY) {
			shoot();
			t.start(actionDelay);
			status = move;
			up = true;
			MessageSystem::getSingleton()->sendMessageGameObject(&Message(PLAY_SOUND), owner_);
			}
		}
	else if (baseY >= highY)
		up = false;
	baseY += tmp;
}


void StomperBehavior::rocketBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;

	nap_vector3 ownerPos = owner_->getPosition(),
		dir = { dest->p_.x_ - ownerPos.x_, 0, dest->p_.z_ - ownerPos.z_ };
	dir = dir.normalize();
	GameObject* o = pRocket->getItem();
	o->setPosition(owner_->getPosition() + dir * 4 + vY * (3 * i));
	MessageSystem::getSingleton()->sendMessageGameObject(&Message(PLAY_SOUND), owner_);
	i++;

	if (i < maxBullets)
		t.start(actionDelay/3);
	else {
		i = 0;
		status = move;
		t.start(actionDelay);
	}
}

void StomperBehavior::shieldBehavior(float time)
{
	if (t.update(time) || t.isActive()) return;

	nap_vector3 ownerPos = owner_->getPosition();
	nap_vector3 dir = { dest->p_.x_ - ownerPos.x_, dest->p_.y_ - ownerPos.y_, dest->p_.z_ - ownerPos.z_ }; //simple vector calc dest - src
	dir = dir.normalize();
	GameObject* o = pSpawn->getItem();
	o->setPosition(owner_->getPosition() + (dir * (10 - i)));
	MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_ENEMY(1),
		GameStateMachine::getSingleton()->currentState()->getGM());
	MessageSystem::getSingleton()->sendMessageGameObject(&Message(PLAY_SOUND), owner_);
	i++;

	if (i < maxBullets)
		t.start(actionDelay);
	else {
		i = 0;
		status = move;
		t.start(actionDelay);
	}
}

void StomperBehavior::shoot()
{
	nap_vector3 ownerPos = owner_->getPosition(), dir;
	GameObject* o;

	for (size_t it = 0; it < positions.size(); it++) {
		dir = directions[it];
		o = pBullet->getItem();
		o->setPosition(ownerPos + dir * 8);
		static_cast<PhysicsComponent*>(o->getComponent("bullet_phys"))->getDynamicBody()->setLinearVelocity((dir * shotSpeed).px());
	}

}

void StomperBehavior::configActive()
{
	status = move;
	lastAttack = 3;
	t.start(actionDelay);
	owner_->setPosition(positions[rand() % positions.size()] + nap_vector3(0, lowY, 0));
}

void StomperBehavior::setUp()
{
	if (isInited()) return;
	setInited();

	//json parameters
	if (FIND(cfg_, "speed")) speed = this->cfg_["speed"];
	if (FIND(cfg_, "shotSpeed")) shotSpeed = this->cfg_["shotSpeed"];
	if (FIND(cfg_, "actionDelay")) actionDelay = this->cfg_["actionDelay"];
	if (FIND(cfg_, "lowY")) lowY = this->cfg_["lowY"];
	if (FIND(cfg_, "highY")) highY = this->cfg_["highY"];
	if (FIND(cfg_, "maxJumps")) maxJumps = this->cfg_["maxJumps"];
	if (FIND(cfg_, "maxBullets")) maxBullets = this->cfg_["maxBullets"];

	t.start(actionDelay);
	pBullet = new nap_Pool("e_bullet");
	pBullet->setDefault(defInitialBullets);
	pBullet->init();
	pSpawn = new nap_Pool("pref_monkey");
	pSpawn->setDefault(defInitialBullets);
	pSpawn->init();
	pRocket = new nap_Pool("e_rocket");
	pRocket->setDefault(defInitialBullets);
	pRocket->init();
	positions.push_back(nap_vector3(55, 0, 0));
	positions.push_back(nap_vector3(0, 0, 55));
	positions.push_back(nap_vector3(-55, 0, 0));
	positions.push_back(nap_vector3(0, 0, -55));
	positions.push_back(nap_vector3(35, 0, 35));
	positions.push_back(nap_vector3(-35, 0, 35));
	positions.push_back(nap_vector3(-35, 0, -35));
	positions.push_back(nap_vector3(35, 0, -35));
	directions.push_back(vX);
	directions.push_back(vZ);
	directions.push_back(nap_vector3(0.7, 0, 0.7));
	directions.push_back(nap_vector3(-0.7, 0, 0.7));
	directions.push_back(nap_vector3(0.7, 0, -0.7));
	directions.push_back(nap_vector3(-0.7, 0, -0.7));
	directions.push_back(nap_vector3(-1, 0, 0));
	directions.push_back(nap_vector3(0, 0, -1));


	string target = this->cfg_["Target"];
	GameObject* o = GameStateMachine::getSingleton()->currentState()->getGameObject(target);

	if (o != nullptr) {
		dest = o->getTransPtr();
	}
	else {
		LogSystem::Log("GameObject " + target + " does not exist in the current state", LogSystem::GAME,  {__FILE__, __LINE__} );
	}
	owner_->setPosition(positions[rand() % positions.size()]);
}

void StomperBehavior::update(GameObject* ent, double time)
{
	owner_->setPosition(nap_vector3(owner_->getPosition().x_, baseY, owner_->getPosition().z_));
	switch (status)
	{
	case move:
		moveBehavior(time);
		break;
	case jump:
		jumpBehavior(time);
		break;
	case rocket:
		rocketBehavior(time);
		break;
	case shield:
		shieldBehavior(time);
		break;
	default:
		break;
	}
}

REGISTER_TYPE(StomperBehavior);
