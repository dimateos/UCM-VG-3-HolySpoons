#include "ComponentFactory.h"
#include <Component.h>

ComponentFactory* ComponentFactory::instance_ = nullptr;

ComponentFactory* ComponentFactory::getSingleton() {
	if (instance_ == nullptr) {
		instance_ = new ComponentFactory();
	}

	return instance_;
}

void ComponentFactory::shutdownSingleton() {
	if (instance_ == nullptr) return;

	delete instance_;
	instance_ = nullptr;
}

// it receives the name of the component and its parameters. When you add a new component to the factory,
// you'll need to know how many parameters that component needs in the constructor. Here is an example:
// the component render always needs two parameters, so you would write -> 
/*
if (componentName == "Render") {
		RenderComponent* rcomp = new RenderComponent(stoi(params[0]), stoi(params[1]));
		return rcomp;
	}
*/
Component* ComponentFactory::ParseComponent(string componentName, std::vector<string> params) {
	// here introduce new components

	return nullptr;
}
