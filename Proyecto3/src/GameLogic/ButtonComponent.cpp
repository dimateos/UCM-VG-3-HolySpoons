#include <RenderSystemInterface.h>
#include "MessageSystem.h"
#include <SDL_events.h>	
#include <iostream>
#include "ButtonComponent.h"

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
	Ogre::OverlayElement* elemt = RenderSystemInterface::getSingleton()->createOverlayElement("Panel", this->cfg_["name"]);

	RenderSystemInterface::getSingleton()->addToPanel(this->cfg_["panelName"], elemt);

	RenderSystemInterface::getSingleton()->setOverlayElementMaterial(elemt, cfg_["materialName"]);

	RenderSystemInterface::getSingleton()->setOverlayElementPosition(elemt, X, Y);

	RenderSystemInterface::getSingleton()->setOverlayElementDimensions(elemt, W, H);
}

bool ButtonComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (evt.button.button == SDL_BUTTON_LEFT) {
			int x_ = evt.button.x, y_ = evt.button.y;
			if (x_ >= X && x_ <= X + W && y_ >= Y && y_ <= Y + H) {
				OnClick();
			}
		}
	}
	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(ButtonComponent);

