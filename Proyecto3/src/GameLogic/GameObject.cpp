#include "GameObject.h"
#include "Component.h"

GameObject::GameObject() :active(true), Components_()
{
}

void GameObject::handleEvents(float time, const Event evt) {
	if (active)
	{
		for (auto comp : Components_)
			comp->handleInput(this, time, evt);
	}
}

void GameObject::update(float time) {
	if (active)
	{
		for (auto comp : Components_)
			comp->update(this, time);
	}
}

void GameObject::addComponent(Component* comp) {
	Components_.push_back(comp);
}

void GameObject::delComponent(Component* comp) {
	std::vector<Component*>::iterator position = std::find(
		Components_.begin(), Components_.end(), comp);

	if (position != Components_.end())
		Components_.erase(position);
}

GameObject::~GameObject() {
	for (auto comp : Components_) {
		if (comp != nullptr) { delete comp; comp = nullptr; delComponent(comp); }
	}
}