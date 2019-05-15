#include "FPSCamera.h"
#include <GlobalConfig.h>

#include "LogSystem.h"

#include "RenderComponent.h"
#include <OgreSceneNode.h>
//#include <OgreNode.h>

//#include <OgreViewport.h>
#include <OgreCamera.h>

#include <SDL_events.h>	//events
#include <Transforms.h>
#include "Messages.h"

void FPSCamera::setUp() {
	if (isInited()) return;
	setInited();

	//cam
	camNode_ = RenderSystemInterface::getSingleton()->getCameraNode();
	cam_ = RenderSystemInterface::getSingleton()->getCamera();

	//set cfg valss
	maxRotY_ = cfg_["maxRotY"];
	zoomFovScale_ = cfg_["zoomFovScale"];
	zoomSensScale_ = cfg_["zoomSensScale"];

	//read fromm global cfg
	rotXspeed_ = GlobalCFG::floats["cam_sensX"] * GlobalCFG::floats["cam_sensBase"];
	rotYspeed_ = GlobalCFG::floats["cam_sensY"] * GlobalCFG::floats["cam_sensBase"];
	baseFovY_ = GlobalCFG::floats["cam_fovY"];
	if (baseFovY_ > GlobalCFG::floats["cam_fovY_max"]) baseFovY_ = GlobalCFG::floats["cam_fovY_max"];
	holdZoom_ = GlobalCFG::flags["hold_zoom"];
}

void FPSCamera::lateSetUp() {
	//postion atm is fixed, then should follow the player
	relativePos_ = nap_vector3(cfg_["relativePos"]);
	camNode_->lookAt(nap_vector3(cfg_["baseLookAt"]).ogre() * ogre_scale, Ogre::Node::TS_WORLD);

	//add spoon ren as child instead of vice versa because the fps cam is permanent
	RenderSystemInterface::getSingleton()->addChild(camNode_, static_cast<RenderComponent*>(owner_->getComponent("spoon_ren"))->getSceneNode());
}

///////////////////////////////////////////////////////////////////////////////

void FPSCamera::update(GameObject * ent, double time) {
	// camera position = entity position + own relative position
	auto pos = ent->getPosition().ogre() + relativePos_.ogre();
	camNode_->setPosition(pos * ogre_scale);

	// entity orientation = camera y orientation
	auto ori = camNode_->getOrientation();
	nap_quat nq = napQUAT(ori);
	ent->setOrientation(nq);

	//avoid flips
	float frame_rotY = rotYspeed_ * -rotY_ * (zoomed_ ? zoomSensScale_ : 1);
	total_rotY_ += frame_rotY;
	if (total_rotY_ > maxRotY_) {
		frame_rotY -= total_rotY_ - maxRotY_;
		total_rotY_ = maxRotY_;
	}
	else if (total_rotY_ < -maxRotY_) {
		frame_rotY -= total_rotY_ + maxRotY_;
		total_rotY_ = -maxRotY_;
	}

	//LogSystem::Log("y: ", total_rotY_);

	//direction
	float frame_rotX = rotXspeed_ * -rotX_ * (zoomed_ ? zoomSensScale_ : 1);
	camNode_->yaw(Ogre::Degree(frame_rotX), Ogre::Node::TS_PARENT);
	camNode_->pitch(Ogre::Degree(frame_rotY), Ogre::Node::TS_LOCAL);

	//reset
	rotX_ = 0.0f;
	rotY_ = 0.0f;
}

bool FPSCamera::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;
	switch (evt.type) {

	case SDL_MOUSEMOTION:
		rotX_ = evt.motion.xrel;
		rotY_ = evt.motion.yrel;
		handled = true;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (evt.button.button == SDL_BUTTON_RIGHT) {
			if (holdZoom_) setZoom(true);
			else setZoom(!zoomed_);
			handled = true;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (evt.button.button == SDL_BUTTON_RIGHT) {
			if (holdZoom_) setZoom(false);
			handled = true;
		}
		break;
	}
	return handled;
}

void FPSCamera::receive(Message * msg) {
	if (msg->id_ == STATE_IN) {
		//hide and capture mouse
		//SDL_ShowCursor(SDL_DISABLE); //no need
		SDL_SetRelativeMouseMode(SDL_TRUE);

		//reset zoom if readded
		setZoom(false);
	}
}

void FPSCamera::setZoom(bool b) {
	zoomed_ = b;
	cam_->setFOVy(Ogre::Degree(baseFovY_ * (zoomed_ ? zoomFovScale_ : 1)));
}

#include "GOFactory.h"
REGISTER_TYPE(FPSCamera);