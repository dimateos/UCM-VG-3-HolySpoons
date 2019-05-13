#include "ButtonSizeChangeComponent.h"

#include <RenderSystemInterface.h>
#include "OgreCamera.h"
#include "OgreViewport.h"
#include "ButtonComponent.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"
#include <SDL_events.h>
#include <OgreOverlayElement.h>

void ButtonSizeChangeComponent::setUp()
{
	if (isInited()) return;
	setInited();

	multiplier = this->cfg_["Multiplier"];
}

void ButtonSizeChangeComponent::lateSetUp() {

	//!no handling if values are in the json / no default fals
	elemt = RenderSystemInterface::getSingleton()->getOverlayElement(static_cast<ButtonComponent*>(this->owner_->getComponent("button"))->getElement()->getName());
	auto vp = RenderSystemInterface::getSingleton()->getCamera()->getViewport();
	X = elemt->getLeft();
	Y = elemt->getTop();
	W = elemt->getWidth();
	H = elemt->getHeight();

	int vpW = vp->getActualWidth(), vpH = vp->getActualHeight();

	bigX = vpW * X;
	bigY = vpH * Y;

	bigW = vpW * W;
	bigH = vpH * H;
}

void ButtonSizeChangeComponent::centerOverlay() {
	auto rsi = RenderSystemInterface::getSingleton();
	auto vp = rsi->getCamera()->getViewport();
	int vpW = vp->getActualWidth(), vpH = vp->getActualHeight();

	bigX = vpW * X;
	bigY = vpH * Y;

	bigW = vpW * W;
	bigH = vpH * H;
}

bool ButtonSizeChangeComponent::handleEvents(GameObject * o, const SDL_Event & evt) {
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

bool ButtonSizeChangeComponent::inside(int x, int y) {
	return x >= bigX && x <= bigX + bigW && y >= bigY && y <= bigY + bigH;
}

void ButtonSizeChangeComponent::onClick() {
	auto rsi = RenderSystemInterface::getSingleton();
	if (outside) {
		//aux vars
		float newW = bigW * multiplier;
		float newH = bigH * multiplier;
		float temp_x = (newW - bigW) / 2.0f;
		float temp_y = (newH - bigH) / 2.0f;

		rsi->setOverlayElementDimensions_abs(elemt, newW, newH);
		rsi->setOverlayElementPosition_abs(elemt, bigX - temp_x, bigY - temp_y);
	}
	else {
		rsi->setOverlayElementDimensions_abs(elemt, bigW, bigH);
		rsi->setOverlayElementPosition_abs(elemt, bigX, bigY);
	}

}

#include "GOFactory.h"
REGISTER_TYPE(ButtonSizeChangeComponent);