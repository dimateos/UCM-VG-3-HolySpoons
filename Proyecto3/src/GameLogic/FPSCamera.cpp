#include "FPSCamera.h"
#include "RenderSystemInterface.h"
#include <OgreNode.h>

#include <iostream>

void FPSCamera::update(GameObject * ent, float time) {
	camNode_->yaw(Degree(rotXspeed_ * time * -rotX_), Node::TS_PARENT);
	camNode_->pitch(Degree(rotYspeed_ * time * -rotY_), Node::TS_LOCAL);
	rotX_ = 0.0f;
	rotY_ = 0.0f;
}

bool FPSCamera::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_MOUSEMOTION) {
		handled = true;
		rotX_ = evt.motion.xrel;
		rotY_ = evt.motion.yrel;
	}

	if (evt.type == SDL_MOUSEBUTTONUP && evt.button.button == SDL_BUTTON_RIGHT) {
		handled = true;
		toggleZoom();
	}

	return handled;
}

void FPSCamera::setUp() {
	//SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	auto rsi = RenderSystemInterface::getSingleton();
	camNode_ = rsi->getCameraNode();
}

void FPSCamera::setDown() {}

void FPSCamera::toggleZoom() {
	auto q = camNode_->getOrientation(),
		qYaw = Quaternion(q.getYaw(), Vector3(0.0f, 1.0f, 0.0f)),
		qPitch = Quaternion(q.getPitch(), Vector3(-1.0f, 0.0f, 0.0f));

	camNode_->translate(qYaw * qPitch * Vector3(0.0f, 0.0f, zoomed), Node::TS_LOCAL);
	//camNode_->translate(Vector3(0.0f, 0.0f, zoomed), Node::TS_LOCAL);
	zoomed *= -1;
}
