#include "ButtonComponent.h"

#include <RenderSystemInterface.h>
#include "OgreCamera.h"
#include "OgreViewport.h"

#include "MessageSystem.h"
#include "GameStateMachine.h"
#include <SDL_events.h>

void ButtonComponent::setUp() {
	X = this->cfg_["X"];
	Y = this->cfg_["Y"];
	W = this->cfg_["W"];
	H = this->cfg_["H"];
	msgType = this->cfg_["msgType"];
	string name = this->cfg_["name"];

	auto rsi = RenderSystemInterface::getSingleton();
	elemt = rsi->createOverlayElement("Panel", name + id().sn_string());
	rsi->addToPanel(this->cfg_["panelName"], elemt);
	rsi->setOverlayElementMaterial(elemt, cfg_["materialName"]);

	centerOverlay();
}

void ButtonComponent::centerOverlay() {
	auto rsi = RenderSystemInterface::getSingleton();
	auto vp = rsi->getCamera()->getViewport();
	int vpW = vp->getActualWidth(), vpH = vp->getActualHeight();

	bigX = vpW * X;
	bigY = vpH * Y;
	rsi->setOverlayElementPosition(elemt, bigX, bigY);

	bigW = vpW * W;
	bigH = vpH * H;
	rsi->setOverlayElementDimensions(elemt, bigW, bigH);
}

bool ButtonComponent::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_MOUSEBUTTONUP) {
		if (evt.button.button == SDL_BUTTON_LEFT && inside(evt.button.x, evt.button.y)) {
			OnClick();
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

void ButtonComponent::OnClick() {
	MessageSystem::getSingleton()->sendMessageGameObject(&Message((MessageId)msgType),
		GameStateMachine::getSingleton()->currentState()->getGM());
}

#include "GOFactory.h"
REGISTER_TYPE(ButtonComponent);