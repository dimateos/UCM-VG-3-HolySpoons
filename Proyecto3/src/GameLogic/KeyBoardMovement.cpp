#include "KeyBoardMovement.h"

#include <Transforms.h>

KeyBoardMovement::KeyBoardMovement(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {}
KeyBoardMovement::~KeyBoardMovement() {}

// updates the go velocity depending on an orientation
void KeyBoardMovement::updateVelocity(nap_vector3 orientation) {
	nap_vector3 dir = owner_->getOrientation().toNapVec3(orientation);
	dir.y_ = 0;
	dir = dir.normalize();
	velocity = velocity + dir * vel_;
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
	jump_ = SDLK_SPACE;

	// velocity sets
	walkVel_ = FINDnRETURN(cfg_, "walkVel", float, 2);
	runVel_ = FINDnRETURN(cfg_, "runVel", float, 4);
	jumpForce_ = FINDnRETURN(cfg_, "jumpForce", float, 4);
	vel_ = walkVel_;

	velocity = vO;
	jumpAccuracy_ = 0.06;
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
		else if (pressedKey == jump_) {
			if (abs(controller_comp->getV().y_) <= jumpAccuracy_) {
				nap_vector3 f = { 0, jumpForce_, 0 };
				controller_comp->addF(f);
			}
		}
		//HAXS
		#if _DEBUG
			else if (pressedKey == SDLK_r) {
				o->setPosition({ 0,10,0 });
			}
		#endif
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
		else handled = false;
	}

	return handled;
}

void KeyBoardMovement::update(GameObject* o, double time) {
	velocity = nap_vector3(0, 0, 0);

	if (!Zaxis.empty()) {
		if (Zaxis.front() == forward_) updateVelocity(vZ*-1);
		if (Zaxis.front() == backward_) updateVelocity(vZ);
	}
	if (!Xaxis.empty()) {
		if (Xaxis.front() == left_) updateVelocity(vX*-1);
		else if (Xaxis.front() == right_) updateVelocity(vX);
	}

	controller_comp->setV(nap_vector3(velocity.x_*time, controller_comp->getV().y_, velocity.z_*time));
}

#include "GOFactory.h"
REGISTER_TYPE(KeyBoardMovement);