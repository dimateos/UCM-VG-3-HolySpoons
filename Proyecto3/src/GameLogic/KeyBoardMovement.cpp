#include "KeyBoardMovement.h"
#include "PhysicsComponent.h"
#include "FPSCamera.h"

KeyBoardMovement::KeyBoardMovement(nap_json const & cfg, GameObject* owner) :Component(cfg, owner)
{
}

void KeyBoardMovement::setUp()
{
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
	if (!cfg_["walkVel"].is_null())
		walkVel_ = cfg_["walkVel"];
	else vel_ = 2;
	if (!cfg_["runVel"].is_null())
		runVel_ = cfg_["runVel"];
	else runVel_ = 4;
	if (!cfg_["jumpForce"].is_null())
		jumpForce_ = cfg_["jumpForce"];
	else jumpForce_ = 4;
	vel_ = walkVel_;

	physBody = static_cast<PhysicsComponent*>(owner_->getComponent("basic_phy"))->getDynamicBody();
	velocity = nap_vector3(0, 0, 0);
}


bool KeyBoardMovement::handleEvents(GameObject * o, const SDL_Event & evt)
{
	bool handled = false;

	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == forward_) {
			Zaxis.push(forward_);
			handled = true;
		}
		else if (pressedKey == left_) {
			Xaxis.push(left_);
			handled = true;
		}
		else if (pressedKey == backward_) {
			Zaxis.push(backward_);
			handled = true;
		}
		else if (pressedKey == right_) {
			Xaxis.push(right_);
			handled = true;
		}
		else if (pressedKey == run_) {
			vel_ = runVel_;
			handled = true;
		}
		else if (pressedKey == jump_ && !jumped) {
			jumped = true;
			nap_vector3 v = { 0, jumpForce_, 0 };
			physBody->addForce(v.px());
			handled = true;
		}
	}

	if (evt.type == SDL_KEYUP) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == forward_) {
			while (!Zaxis.empty())Zaxis.pop();
			handled = true;
		}
		else if (pressedKey == left_) {
			while (!Xaxis.empty())Xaxis.pop();
			handled = true;
		}
		else if (pressedKey == backward_) {
			while (!Zaxis.empty())Zaxis.pop();
			handled = true;
		}
		else if (pressedKey == right_) {
			while (!Xaxis.empty())Xaxis.pop();
			handled = true;
		}
		else if (pressedKey == run_) {
			vel_ = walkVel_;
			handled = true;
		}
	}

	//si no hay teclas en la pila la velocidad se para
	if (Xaxis.empty()) {
		velocity = nap_vector3(0, 0, 0);
	}
	else {//si hay teclas en la pila se mira cual es y se mueve en esa direccion
		if (Xaxis.top() == left_) {
			nap_vector3 dir = o->getOrientation().toNapVec3(nap_vector3(-1, 0, 0));
			dir.y_ = 0;
			velocity = velocity + dir * vel_;
		}
		else if (Xaxis.top() == right_) {
			nap_vector3 dir = o->getOrientation().toNapVec3(nap_vector3(1, 0, 0));
			dir.y_ = 0;
			velocity = velocity + dir * vel_;
		}
	}

	if (Zaxis.empty()) {
		velocity = nap_vector3(0, 0, 0);
	}
	else {//si hay teclas en la pila se mira cual es y se mueve en esa direccion
		if (Zaxis.top() == forward_) {
			nap_vector3 dir = o->getOrientation().toNapVec3(nap_vector3(0, 0, -1));
			dir.y_ = 0;
			velocity = velocity + dir * vel_;
		}
		if (Zaxis.top() == backward_) {
			nap_vector3 dir = o->getOrientation().toNapVec3(nap_vector3(0, 0, 1));
			dir.y_ = 0;
			velocity = velocity + dir * vel_;
		}

		return handled;
	}
}

void KeyBoardMovement::update(GameObject* o, double time) {
	physBody->setLinearVelocity(velocity.px());
}

KeyBoardMovement::~KeyBoardMovement()
{
}
