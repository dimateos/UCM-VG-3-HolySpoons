#include "FPSCamera.h"
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>

void FPSCamera::setUp() {
	if (isInited()) return;
	setInited();

	//hide and capture mouse
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	//cam
	camNode_ = RenderSystemInterface::getSingleton()->getCameraNode();
	//camNode_->setDirection(nap_vector3(cfg_["baseDir"]).ogre());

	//postion atm is fixed, then should follow the player
	camNode_->setPosition(nap_vector3(cfg_["basePos"]).ogre() * ogre_scale);
	camNode_->lookAt(nap_vector3(cfg_["baseLookAt"]).ogre() * ogre_scale, Ogre::Node::TS_WORLD);
}

///////////////////////////////////////////////////////////////////////////////

void FPSCamera::update(GameObject * ent, float time) {
	//direction
	camNode_->yaw(Ogre::Degree(cfg_["rotXspeed"] * time * -rotX_), Ogre::Node::TS_PARENT);
	camNode_->pitch(Ogre::Degree(cfg_["rotYspeed"] * time * -rotY_), Ogre::Node::TS_LOCAL);
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
			toggleZoom();
			handled = true;
		}
		break;
	}
	return handled;
}

void FPSCamera::loadParameters(std::vector<string> parameters) {
	// TO COMPLETE (depending on the needed parameters)
}

void FPSCamera::toggleZoom() {
	camNode_->translate(-vZ.ogre() * (zoom ? -1 : 1) * cfg_["zoomed"] * ogre_scale, Ogre::Node::TS_LOCAL);
	zoom = !zoom;
}