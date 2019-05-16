#include "PhysicsControllerComponent.h"
#include "LogSystem.h"

#include <PhysicsSystemManager.h>
#include <ControllerReporter.h>
#include <SDL_events.h>
#include <Transforms.h>

#include <PxRigidDynamic.h>
#include <characterkinematic\PxCapsuleController.h>

//some global cfg?
#define BaseRadius 2.0f
#define BaseHeight 3.0f
#define BaseDens 1.0		//atm mass not defined
#define BaseImass 1.0		//inverse
#define BaseLinDamp 0.05	//def 0.0 and 1 max

#define BaseContactOffset 0.01f
#define BaseStepOffset 0.05f
#define BaseMinDist 0.001

#define BaseSlopeLimit 0.0f
#define BaseInvisibleWallsHeight 0.0f
#define BaseMaxJumpHeight 0.0f

void PhysicsControllerComponent::setUp() {
	if (isInited()) return;
	setInited();

	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();

	PxCapsuleControllerDesc desc;
	//desc.type = PxControllerShapeType::eCAPSULE;

	//pos
	PxExtendedVec3 pos(.0, .0, .0);
	pos += owner_->getPosition().px();
	desc.position = pos;
	desc.upDirection = vY.px();

	//shape
	desc.radius = FINDnRETURN(cfg_, "radius", float, BaseRadius);
	desc.height = FINDnRETURN(cfg_, "height", float, BaseHeight);
	std::string mat = FINDnRETURN_s(cfg_, "material", BaseMat);
	desc.material = physicsManager->getMaterial(mat);
	desc.density = FINDnRETURN(cfg_, "dens", float, BaseDens); //no update mass?
	i_mass = FINDnRETURN(cfg_, "i_mass", float, BaseImass);
	damping_ = FINDnRETURN(cfg_, "damping", float, BaseLinDamp);

	//main behaviour
	desc.contactOffset = FINDnRETURN(cfg_, "contactOffset", float, BaseContactOffset);
	desc.stepOffset = FINDnRETURN(cfg_, "stepOffset", float, BaseStepOffset);
	minDist = FINDnRETURN(cfg_, "minDist", float, BaseMinDist);

	//more specific behaviour
	desc.slopeLimit = FINDnRETURN(cfg_, "slopeLimit", float, BaseSlopeLimit);
	desc.invisibleWallHeight = FINDnRETURN(cfg_, "invisibleWallsHeight", float, BaseInvisibleWallsHeight);
	desc.maxJumpHeight = FINDnRETURN(cfg_, "maxJumpHeight", float, BaseMaxJumpHeight);

	//collisions
	desc.reportCallback = new ControllerReporter(&ud_);
	//desc.behaviorCallback = this;

	//create the controller
	controller_ = physicsManager->createController(&desc);

	//custom gravity
	g_ = FINDnRETURN(cfg_, "g", nap_vector3, pxG);

	updateUserData();
	configActive();
	//LogSystem::Log("scale: ", owner_->getScale().json(), LogSystem::CONTROLLER);
}

void PhysicsControllerComponent::invalidateChache() {
	controller_->invalidateCache();
}

void PhysicsControllerComponent::setDown() {
	//release the controller (which releases the actor, shape etc)
	controller_->release();
	if (ud_ != nullptr) delete ud_;
}

void PhysicsControllerComponent::configActive() {
	invalidateChache();
	getActor()->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !active_);
	controller_->setPosition((!active_ ? baseTransPos : owner_->getPosition()).pxEx());
}

void PhysicsControllerComponent::updateUserData() {
	if (ud_ != nullptr) delete ud_;
	ud_ = new nap_userData(owner_->getTransPtr(), owner_->getCollisionListeners(), owner_->idPtr(), false);

	//controller_->setUserData(ud_); //unused atm
	getActor()->userData = ud_;
}

PxRigidActor * PhysicsControllerComponent::getActor() {
	return controller_->getActor();
}

///////////////////////////////////////////////////////////////////////////////

void PhysicsControllerComponent::update(GameObject * o, double time) {
	//update with constants v
	v_ = v_ + (a_ + g_) * time;
	v_ = v_ * powf(1 - damping_, time);

	//update puntual impulses
	v_ = v_ + i_ * i_mass;
	i_ = vO;

	//update pos
	nap_vector3 prevPos = napVEC3(controller_->getPosition());
	nap_vector3 newPos = prevPos + v_ * time;

	//calculate disp
	nap_vector3 disp = newPos - prevPos;

	//MOVE THE CONTROLLER
	controller_->move(disp.px(), minDist, time, NULL, NULL);

	//integrate real v
	nap_vector3 finalPos = napVEC3(controller_->getPosition());
	v_ = (finalPos - prevPos) / time;

	//LogSystem::Log("prevPos: ", prevPos.json(), LogSystem::CONTROLLER);
	//LogSystem::Log("newPos: ", newPos.json(), LogSystem::CONTROLLER);
	//LogSystem::Log("finalPos: ", finalPos.json(), LogSystem::CONTROLLER);
	//LogSystem::Log("finalV: ", v_.json(), LogSystem::CONTROLLER);
}

void PhysicsControllerComponent::late_update(GameObject * o, double time) {
	//the controller ignores changes on orientation
	if (o->getUpToDate(upToDate::pos, upToDate::PHYS)) return;

	controller_->setPosition(o->getPosition().pxEx());
	o->setUpToDate(upToDate::pos, upToDate::PHYS);
}

bool PhysicsControllerComponent::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

#if _DEBUG //HAXS
	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == SDLK_c) {
			LogSystem::Log("Manually invalidated chache", LogSystem::CONTROLLER);
			invalidateChache();
			handled = true;
		}
		else if (pressedKey == SDLK_l) {
			LogSystem::Log("Player pos: ", owner_->getPosition().json(), LogSystem::CONTROLLER);
			handled = true;
		}
	}
#endif

	return handled;
}

#include "GOFactory.h"
REGISTER_TYPE(PhysicsControllerComponent);