#include "KeyBoardMovement.h"
#include "PhysicsComponent.h"
#include "FPSCamera.h"

KeyBoardMovement::KeyBoardMovement(nap_json const & cfg, GameObject* owner) :Component(cfg, owner)
{
}

bool KeyBoardMovement::handleEvents(GameObject * o, const SDL_Event & evt)
{
	bool handled = false;

	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;
		PhysicsComponent* physComp = static_cast<PhysicsComponent*>(o->getComponent("basic_phy"));

		if (pressedKey == forward_) {
			nap_vector3 dir = o->getOrientation().toNapVec3();
			physComp->getDynamicBody()->setLinearVelocity((dir.px()) * 2);
			handled = true;
		}
		else if (pressedKey == left_) {
			handled = true;
		}
		else if (pressedKey == backward_) {
			handled = true;
		}
		else if (pressedKey == right_) {
			handled = true;
		}
		else if (pressedKey == run_) {
			vel_ = runVel_;
			handled = true;
		}
		else if (pressedKey == jump_ && !jumped) {
			jumped = true;
			nap_vector3 v = { 0, jumpForce_, 0 };
			physComp->getDynamicBody()->addForce(v.px());
			handled = true;
		}
	}

	if (evt.type == SDL_KEYUP) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == forward_) {
			handled = true;
		}
		else if (pressedKey == left_) {
			handled = true;
		}
		else if (pressedKey == backward_) {
			handled = true;
		}
		else if (pressedKey == right_) {
			handled = true;
		}
		else if (pressedKey == run_) {
			vel_ = walkVel_;
			handled = true;
		}
	}

	return handled;
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
}


KeyBoardMovement::~KeyBoardMovement()
{
}
