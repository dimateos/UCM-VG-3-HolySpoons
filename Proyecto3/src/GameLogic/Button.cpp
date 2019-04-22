#include "Button.h"
#include "MessageSystem.h"
#include <iostream>

Button::Button(nap_json const & cfg, GameObject* owner) : Component(cfg,owner) {

	X = cfg["X"];
	Y = cfg["Y"];
	W = cfg["W"];
	H = cfg["H"];
	msgType = cfg["msgType"];
}

void Button::OnClick()
{
	//MessageSystem::getSingleton()->sendMessageType(&Message((MessageId)msgType));
	std::cout << "Boton Pulsado" << std::endl;
}

void Button::setUp()
{
}

#include "GOFactory.h"
REGISTER_TYPE(Button);

