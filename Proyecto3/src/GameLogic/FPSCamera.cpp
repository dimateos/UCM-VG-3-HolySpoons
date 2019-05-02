#include "FPSCamera.h"
#include "LogSystem.h"
#include "RenderComponent.h"

#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>

#include <Transforms.h>
#include <SDL_events.h>	//events
#include "Messages.h"

void FPSCamera::setUp() {
	if (isInited()) return;
	setInited();

	//cam
	camNode_ = RenderSystemInterface::getSingleton()->getCameraNode();
	//camNode_->setDirection(nap_vector3(cfg_["baseDir"]).ogre());

	//set cfg vals
	rotXspeed_ = cfg_["rotXspeed"];
	rotYspeed_ = cfg_["rotYspeed"];
	maxRotY_ = cfg_["maxRotY"];
	zoomed_ = cfg_["zoomed"];
}

void FPSCamera::lateSetUp() {
	//postion atm is fixed, then should follow the player
	relativePos = nap_vector3(cfg_["relativePos"]);
	camNode_->lookAt(nap_vector3(cfg_["baseLookAt"]).ogre() * ogre_scale, Ogre::Node::TS_WORLD);

	//add spoon ren as child
	RenderSystemInterface::getSingleton()->addChild(camNode_, static_cast<RenderComponent*>(owner_->getComponent("spoon_ren"))->getSceneNode());
}

///////////////////////////////////////////////////////////////////////////////

void FPSCamera::update(GameObject * ent, double time) {
	// camera position = entity position + own relative position
	camNode_->setPosition((ent->getPosition().ogre() + relativePos.ogre()) * ogre_scale);

	// entity orientation = camera y orientation
	auto ori = camNode_->getOrientation();
	nap_quat nq = napQUAT(ori);
	ent->setOrientation(nq);

	//avoid flips
	float frame_rotY = rotYspeed_ * time * -rotY_;
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
	camNode_->yaw(Ogre::Degree(rotXspeed_ * time * -rotX_), Ogre::Node::TS_PARENT);
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

	case SDL_MOUSEBUTTONUP:
		if (evt.button.button == SDL_BUTTON_RIGHT) {
			//toggleZoom(); //for now
			handled = true;
		}
		break;
	}
	return handled;
}

void FPSCamera::toggleZoom() {
	camNode_->translate(-vZ.ogre() * (zoom ? -1 : 1) * zoomed_ * ogre_scale, Ogre::Node::TS_LOCAL);
	zoom = !zoom;
}

void FPSCamera::receive(Message * msg)
{
	if (msg->id_ == STATE_CHANGED) {
		//hide and capture mouse
		SDL_ShowCursor(SDL_DISABLE);
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(FPSCamera);