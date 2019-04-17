#include "Component.h"
#include "GOFactory.h"

std::map<string, GOFactory*>* Component::factories = nullptr;

void Component::registerType(const string& name, GOFactory* factory) {
	if (factories == nullptr)factories = new std::map<string, GOFactory*>();
	Component::factories->insert({ name, factory });
}