#include "HoverButton.h"

#include <RenderSystemInterface.h>
#include "OgreCamera.h"
#include "OgreViewport.h"

#include "MessageSystem.h"
#include "GameStateMachine.h"
#include <SDL_events.h>

void HoverButton::setUp() {
	if (isInited()) return;
	setInited();

	//!no handling if values are in the json / no default fals

	//dimensions
	X = this->cfg_["X"];
	Y = this->cfg_["Y"];
	W = this->cfg_["W"];
	H = this->cfg_["H"];

	buttonMat = cfg_["materialName"];
	imgMat = cfg_["imgMat"];

	//name and type
	string name = this->cfg_["name"];
	//enterMsg = this->cfg_["enterMsg"];
	//exitMsg = this->cfg_["exitMsg"];

	// name of the component that will listen the message (push or pop)
	if (FIND(cfg_, "listener")) {
		string s = this->cfg_["listener"];
		listener = s;
	}
	else listener = ""; // listener not specified (there is only one possible listener)

	//create the panel
	auto rsi = RenderSystemInterface::getSingleton();
	elemt = rsi->createOverlayElement("Panel", name + id().sn_string());
	rsi->addToPanel(this->cfg_["panelName"], elemt);
	rsi->setOverlayElementMaterial(elemt, imgMat);
	centerOverlay();
}

//center the overlay based on its dimensions
void HoverButton::centerOverlay() {
	auto rsi = RenderSystemInterface::getSingleton();
	auto vp = rsi->getCamera()->getViewport();
	int vpW = vp->getActualWidth(), vpH = vp->getActualHeight();

	bigX = vpW * X;
	bigY = vpH * Y;
	rsi->setOverlayElementPosition_abs(elemt, bigX, bigY);

	bigW = vpW * W;
	bigH = vpH * H;
	rsi->setOverlayElementDimensions_abs(elemt, bigW, bigH);
}

bool HoverButton::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_MOUSEMOTION) {
		if (outside && inside(evt.button.x, evt.button.y)) { //if previously the mouse was outside the button, i check for when it enters it's range
			onClick();
			outside = false;
			handled = true;
		}
		else if (!outside && !inside(evt.button.x, evt.button.y)) { //else i check when it leaves the button's range
			onClick();
			outside = true;
			handled = true;
		}
	}
	else if (evt.type == SDL_WINDOWEVENT) {
		if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
			centerOverlay();
		}
	}

	return handled;
}

void HoverButton::receive(Message * msg) {
	if (msg->id_ == STATE_IN) {
		centerOverlay();
	}
}

bool HoverButton::inside(int x, int y) {
	return x >= bigX && x <= bigX + bigW && y >= bigY && y <= bigY + bigH;
}

void HoverButton::onClick() {
	/*
	Message msg = Message((MessageId)0);
	if(outside) msg = (MessageId)enterMsg;
	else  msg = (MessageId)exitMsg;

	if (listener == "") { // if listener is not specified (only one possible listener in the entire gm)
		MessageSystem::getSingleton()->sendMessageGameObject(&msg,
			GameStateMachine::getSingleton()->currentState()->getGM());
	}
	// if listener is specified we send the message to that component of the gm
	else MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&msg,
		GameStateMachine::getSingleton()->currentState()->getGM(), listener);
	*/
	auto rsi = RenderSystemInterface::getSingleton();
	if (outside) {
		rsi->setOverlayElementMaterial(elemt, buttonMat);
		rsi->setOverlayElementDimensions_abs(elemt, bigW * 5, bigH * 5);
		rsi->setOverlayElementPosition_rel(elemt, 0, 0);
	}
	else {
		rsi->setOverlayElementMaterial(elemt, imgMat);
		rsi->setOverlayElementDimensions_abs(elemt, bigW, bigH);
		rsi->setOverlayElementPosition_abs(elemt, bigX, bigY);
	}

}

#include "GOFactory.h"
REGISTER_TYPE(HoverButton);