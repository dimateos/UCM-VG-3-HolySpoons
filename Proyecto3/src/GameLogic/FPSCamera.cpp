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
	camNode_->setPosition(500, 500, 4000);
	camNode_->lookAt(Ogre::Vector3(0.0f, -500.0f, 0.0f), Ogre::Node::TS_WORLD);
}

///////////////////////////////////////////////////////////////////////////////

void FPSCamera::update(GameObject * ent, float time) {
	camNode_->yaw(Ogre::Degree(rotXspeed_ * time * -rotX_), Ogre::Node::TS_PARENT);
	camNode_->pitch(Ogre::Degree(rotYspeed_ * time * -rotY_), Ogre::Node::TS_LOCAL);
	rotX_ = 0.0f;
	rotY_ = 0.0f;
}

bool FPSCamera::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	switch (evt.type) {
	//case SDL_WINDOWEVENT:
	//	if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
	//		handled = true;
	//	}
	//	break;

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

void FPSCamera::loadParameters(std::vector<string> parameters)
{
	// TO COMPLETE (depending on the needed parameters)
}

void FPSCamera::toggleZoom() {
	camNode_->translate(-vZ.ogre() * zoomed, Ogre::Node::TS_LOCAL);
	zoomed *= -1;
}