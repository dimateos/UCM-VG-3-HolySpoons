#include "FPSCamera.h"

#include "RenderSystemInterface.h"

void FPSCamera::setUp() {
	//hide and capture mouse
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	//cam
	camNode_ = RenderSystemInterface::getSingleton()->getCameraNode();
	camNode_->setPosition(500, 500, 4000);
	camNode_->lookAt(Ogre::Vector3(0.0f, -500.0f, 0.0f), Ogre::Node::TS_WORLD);

	//crosshair
	x = new OgreText();
	x->setText("- x -");
	x->setTextColour(1, 1, 1, 1);
	updateCrosshair();
}

void FPSCamera::setDown() {
	delete x;
}

void FPSCamera::updateCrosshair() {
	Viewport* vp = RenderSystemInterface::getSingleton()->getViewport();
	x->setTextPosition(vp->getActualWidth() / 2 - 50, vp->getActualHeight() / 2 - 25);
}

///////////////////////////////////////////////////////////////////////////////

void FPSCamera::update(GameObject * ent, float time) {
	camNode_->yaw(Degree(rotXspeed_ * time * -rotX_), Node::TS_PARENT);
	camNode_->pitch(Degree(rotYspeed_ * time * -rotY_), Node::TS_LOCAL);
	rotX_ = 0.0f;
	rotY_ = 0.0f;
}

bool FPSCamera::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;

	switch (evt.type) {
	case SDL_WINDOWEVENT:
		if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
			updateCrosshair();
			handled = true;
		}
		break;

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

void FPSCamera::toggleZoom() {
	camNode_->translate(-oZ * zoomed, Node::TS_LOCAL);
	zoomed *= -1;
}