#include "KeyBoardMovement.h"
#include <LogSystem.h>
#include <GlobalConfig.h>

#include "MessageSystem.h"
#include "PhysicsControllerComponent.h"
#include <SDL_events.h>
#include <Transforms.h>

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

	//read the keys from global cfg
	forward_ = GlobalCFG::keys["FORWARD"];
	backward_ = GlobalCFG::keys["BACKWARD"];
	left_ = GlobalCFG::keys["LEFT"];
	right_ = GlobalCFG::keys["RIGHT"];
	jumpKey_ = GlobalCFG::keys["JUMP"];
	run_ = GlobalCFG::keys["RUN"];
	holdSprint_ = GlobalCFG::flags["hold_sprint"];

	// velocity sets
	walkVel_ = FINDnRETURN(cfg_, "walkVel", float, 20);
	runVel_ = FINDnRETURN(cfg_, "runVel", float, 40);
	walkIncr = FINDnRETURN(cfg_, "walkIncr", float, 2);
	runIncr = FINDnRETURN(cfg_, "runIncr", float, 2);
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
		handled = true;
		SDL_Keycode pressedKey = evt.key.keysym.sym;

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
			if (holdSprint_) setSprinting();
			else setSprinting(!sprinting_);
		}
		else if (pressedKey == jumpKey_) {
			jumping_ = true;
		}

		else handled = false;
	}

	else if (evt.type == SDL_KEYUP) {
		handled = true;
		SDL_Keycode pressedKey = evt.key.keysym.sym;

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
			if (holdSprint_) setSprinting(false);
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

void KeyBoardMovement::receive(Message * msg) {
	if (msg->id_ == STATE_IN) {
		//clear the buffers
		jumping_ = false;
		sprinting_ = false;
		Zaxis.clear();
		Xaxis.clear();
	}
	else if (msg->id_ == RESET_JUMP) {
		jumping_ = false;
		sprinting_ = false;
	}
	else if (msg->id_ == ADD_SPEED) {
		walkVel_ += walkIncr;
		runVel_ += runIncr;
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
	if (velocity != vO) velocity = velocity.normalize() * (sprinting_ ? runVel_ : walkVel_);

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

void KeyBoardMovement::setSprinting(bool b) {
	sprinting_ = b;
	MessageSystem::getSingleton()->sendMessageGameObject(&Message(sprinting_ ? SPRINT_ON : SPRINT_OFF), owner_);
}


#include "GOFactory.h"
REGISTER_TYPE(KeyBoardMovement);