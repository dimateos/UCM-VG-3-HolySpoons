#include "GameObject.h"
#include "Component.h"

GameObject::GameObject() : Activable(true), Components_() {}

bool GameObject::handleEvents(const SDL_Event evt) {
	bool handled = false;
	auto it = Components_.begin();
	while (!handled && it != Components_.end()) {
		if ((*it)->isActive()) handled = (*it)->handleEvents(this, evt);
		it++;
	}

	return handled;
}

void GameObject::update(float time) {
	for (auto comp : Components_)
		if(comp != nullptr && comp->isActive())comp->update(this, time);
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
		if (comp != nullptr)delete comp;
	}

	Components_.clear();
}