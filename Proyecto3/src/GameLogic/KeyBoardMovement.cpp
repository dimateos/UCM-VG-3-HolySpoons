#include "KeyBoardMovement.h"
#include <LogSystem.h>

#include <Transforms.h>

KeyBoardMovement::KeyBoardMovement(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), CollisionListener(owner) {}
KeyBoardMovement::~KeyBoardMovement() {}

// updates the go velocity depending on an orientation
void KeyBoardMovement::updateVelocity(nap_vector3 orientation) {
	nap_vector3 dir = owner_->getOrientation().toNapVec3(orientation);
	dir.y_ = 0;
	dir = dir.normalize();
	velocity = velocity + dir;
}

void KeyBoardMovement::setUp() {
	if (isInited()) return;
	setInited();

	// always these keycodes, if you want to use others in the .json
	// you must do a factory of them
	forward_ = SDLK_w;
	left_ = SDLK_a;
	backward_ = SDLK_s;
	right_ = SDLK_d;
	run_ = SDLK_LSHIFT;
	jumpKey_ = SDLK_SPACE;

	// velocity sets
	walkVel_ = FINDnRETURN(cfg_, "walkVel", float, 2);
	runVel_ = FINDnRETURN(cfg_, "runVel", float, 4);
	vel_ = walkVel_;
	velocity = vO;

	//jump
	jumpImpulse_ = FINDnRETURN(cfg_, "jumpImpulse", float, 4);
	for (auto group : cfg_["jumpResetersGroups"]) jumpResetersGroups_.push_back(group);
}

void KeyBoardMovement::lateSetUp() {
	// physics component
	controller_comp = static_cast<PhysicsControllerComponent*>(owner_->getComponent("controller_phy"));
}

bool KeyBoardMovement::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		handled = true;
		if (pressedKey == forward_) {
			Zaxis.push_front(forward_);
		}
		else if (pressedKey == left_) {
			Xaxis.push_front(left_);
		}
		else if (pressedKey == backward_) {
			Zaxis.push_front(backward_);
		}
		else if (pressedKey == right_) {
			Xaxis.push_front(right_);
		}
		else if (pressedKey == run_) {
			vel_ = runVel_;
		}
		else if (pressedKey == jumpKey_) {
			jumping_ = true;
		}
		else handled = false;
	}

	else if (evt.type == SDL_KEYUP) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		handled = true;
		if (pressedKey == forward_) {
			Zaxis.remove(forward_);
		}
		else if (pressedKey == left_) {
			Xaxis.remove(left_);
		}
		else if (pressedKey == backward_) {
			Zaxis.remove(backward_);
		}
		else if (pressedKey == right_) {
			Xaxis.remove(right_);
		}
		else if (pressedKey == run_) {
			vel_ = walkVel_;
		}
		else if (pressedKey == jumpKey_) {
			jumping_ = false;
		}
		else handled = false;
	}

	return handled;
}

void KeyBoardMovement::onCollision(ID * other) {
	if (jump_available_) return;

	//check if is a jump reseter
	for (auto & group : jumpResetersGroups_) {
		if (other->group_ == group) {
			jump_available_ = true;
			//LogSystem::Log("JUMP RESTORED");
			break;
		}
	}
}

void KeyBoardMovement::update(GameObject* o, double time) {
	if (jumping_) jump();

	velocity = vO;
	if (!Zaxis.empty()) {
		if (Zaxis.front() == forward_) updateVelocity(vZ*-1);
		else if (Zaxis.front() == backward_) updateVelocity(vZ);
	}
	if (!Xaxis.empty()) {
		if (Xaxis.front() == left_) updateVelocity(vX*-1);
		else if (Xaxis.front() == right_) updateVelocity(vX);
	}

	//if velocity edited
	if (velocity != vO) velocity = velocity.normalize() * vel_;

	//if no edited anyway way stop the player
	controller_comp->setV(nap_vector3(velocity.x_, controller_comp->getV().y_, velocity.z_));
}

void KeyBoardMovement::jump() {
	if (!jump_available_) return;
	//LogSystem::Log("JUMP");

	nap_vector3 f = { 0, jumpImpulse_, 0 };
	controller_comp->addI(f);
	controller_comp->setV(vO);
	jump_available_ = false;
}


#include "GOFactory.h"
REGISTER_TYPE(KeyBoardMovement);