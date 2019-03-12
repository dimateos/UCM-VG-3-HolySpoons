#include "GameObject.h"
#include "Component.h"

GameObject::GameObject() :active(true), Components_() {}

bool GameObject::handleEvents(const SDL_Event evt) {
	bool handled = false;
	auto it = Components_.begin();
	while (!handled && it != Components_.end()) {
		handled = (*it)->handleEvents(this, evt);
		it++;
	}

	return handled;
}

void GameObject::update(float time) {
	for (auto comp : Components_)
		if(comp != nullptr)comp->update(this, time);
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