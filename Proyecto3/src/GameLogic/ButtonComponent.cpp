#include "ButtonComponent.h"

#include <RenderSystemInterface.h>
#include "OgreCamera.h"
#include "OgreViewport.h"

#include "MessageSystem.h"
#include "GameStateMachine.h"
#include <SDL_events.h>

void ButtonComponent::setUp() {
	if (isInited()) return;
	setInited();

	//!no handling if values are in the json / no default fals

	//dimensions
	X = this->cfg_["X"];
	Y = this->cfg_["Y"];
	W = this->cfg_["W"];
	H = this->cfg_["H"];

	//name and type
	string name = this->cfg_["name"];
	msgType = this->cfg_["msgType"];

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
	rsi->setOverlayElementMaterial(elemt, cfg_["materialName"]);
	centerOverlay();
}

//center the overlay based on its dimensions
void ButtonComponent::centerOverlay() {
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

bool ButtonComponent::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_MOUSEBUTTONUP) {
		if (evt.button.button == SDL_BUTTON_LEFT && inside(evt.button.x, evt.button.y)) {
			onClick();
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

bool ButtonComponent::inside(int x, int y) {
	return x >= bigX && x <= bigX + bigW && y >= bigY && y <= bigY + bigH;
}

void ButtonComponent::onClick() {
	if (listener == "") { // if listener is not specified (only one possible listener in the entire gm)
		MessageSystem::getSingleton()->sendMessageGameObject(&Message((MessageId)msgType),
			GameStateMachine::getSingleton()->currentState()->getGM());
	}
	// if listener is specified we send the message to that component of the gm
	else MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message((MessageId)msgType),
		GameStateMachine::getSingleton()->currentState()->getGM(), listener);
}

#include "GOFactory.h"
REGISTER_TYPE(ButtonComponent);