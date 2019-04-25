#include <RenderSystemInterface.h>
#include "MessageSystem.h"
#include <SDL_events.h>	
#include <iostream>
#include "ButtonComponent.h"
#include "OgreCamera.h"
#include "OgreViewport.h"

ButtonComponent::ButtonComponent(nap_json const & cfg, GameObject* owner) : Component(cfg,owner) {

	X = cfg["X"];
	Y = cfg["Y"];
	W = cfg["W"];
	H = cfg["H"];
	msgType = cfg["msgType"];
	
}

void ButtonComponent::OnClick()
{
	MessageSystem::getSingleton()->sendMessage(&Message((MessageId)msgType));
	std::cout << "Boton Pulsado" << std::endl;
}

void ButtonComponent::setUp()
{
	elemt = RenderSystemInterface::getSingleton()->createOverlayElement("Panel", this->cfg_["name"]);

	RenderSystemInterface::getSingleton()->addToPanel(this->cfg_["panelName"], elemt);

	RenderSystemInterface::getSingleton()->setOverlayElementMaterial(elemt, cfg_["materialName"]);

	bigX = RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualWidth()* X;
	bigY = RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualHeight() * Y;
	RenderSystemInterface::getSingleton()->setOverlayElementPosition(elemt,bigX, bigY);

	RenderSystemInterface::getSingleton()->setOverlayElementDimensions(elemt, W, H);
}

bool ButtonComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (evt.button.button == SDL_BUTTON_LEFT) {
			int x_ = evt.button.x, y_ = evt.button.y;
			if (x_ >= bigX && x_ <= bigX + W && y_ >= bigY && y_ <= bigY + H) {
				OnClick();
			}
		}
	}
	else if (evt.type == SDL_WINDOWEVENT) {
		if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
			bigX = RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualWidth() * X;
			bigY = RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualHeight() * Y;
			RenderSystemInterface::getSingleton()->setOverlayElementPosition(elemt, bigX, bigY);
		}
	}
	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(ButtonComponent);

