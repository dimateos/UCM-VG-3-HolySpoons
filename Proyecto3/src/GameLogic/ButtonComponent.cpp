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
	//MessageSystem::getSingleton()->sendMessageType(&Message((MessageId)msgType));
	std::cout << "Boton Pulsado" << std::endl;
}

void ButtonComponent::setUp()
{
	Ogre::OverlayElement* elemt = RenderSystemInterface::getSingleton()->createOverlayElement("Panel", "kk");

	RenderSystemInterface::getSingleton()->addToPanel(this->cfg_["panelName"], elemt);

	RenderSystemInterface::getSingleton()->setOverlayElementMaterial(elemt, cfg_["materialName"]);

	RenderSystemInterface::getSingleton()->setOverlayElementPosition(elemt, X, Y);

	RenderSystemInterface::getSingleton()->setOverlayElementDimensions(elemt, W, H);
}

bool ButtonComponent::handleEvents(GameObject * o, const SDL_Event & evt)
{
	if (evt.type == SDL_BUTTON_LEFT) {
		int x_ = 0, y_ = 0;
		SDL_GetMouseState(&x_, &y_);
		if (x_ >= X && x_ <= X + W && y_ >= Y && y_ <= Y + H) {
			OnClick();
		}
	}
	return false;
}

#include "GOFactory.h"
REGISTER_TYPE(ButtonComponent);

