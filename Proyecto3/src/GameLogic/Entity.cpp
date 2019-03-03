#include "Entity.h"
#include "Component.h"

using namespace Proyecto;

Entity::Entity() :active(true), Components_()
{
}

void Entity::handleEvents(float time, const Event evt) {
	if (active)
	{
		for (auto comp : Components_)
			comp->handleInput(this, time, evt);
	}
}

void Entity::update(float time) {
	if (active)
	{
		for (auto comp : Components_)
			comp->update(this, time);
	}
}

void Entity::addComponent(Component* comp) {
	Components_.push_back(comp);
}

void Entity::delComponent(Component* comp) {
	std::vector<Component*>::iterator position = std::find(
		Components_.begin(), Components_.end(), comp);

	if (position != Components_.end())
		Components_.erase(position);
}

Entity::~Entity() {
	for (auto comp : Components_) {
		if (comp != nullptr) { delete comp; comp = nullptr; delComponent(comp); }
	}
}