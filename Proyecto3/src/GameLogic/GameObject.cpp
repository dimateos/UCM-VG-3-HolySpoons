#include "GameObject.h"
#include "Component.h"

GameObject::GameObject() : Activable(), components_() {}
GameObject::GameObject(std::list<Component*> comps) : Activable(), components_(comps) {}

GameObject::~GameObject() {
	for (auto comp : components_) {
		if (comp != nullptr)delete comp;
	}

	components_.clear();
}

///////////////////////////////////////////////////////////////////////////////

bool GameObject::handleEvents(const SDL_Event evt) {
	bool handled = false;
	auto it = components_.begin();
	while (!handled && it != components_.end()) {
		if ((*it)->isActive()) handled = (*it)->handleEvents(this, evt);
		it++;
	}

	return handled;
}

void GameObject::update(float time) {
	for (auto comp : components_)
		if (comp != nullptr && comp->isActive())comp->update(this, time);
}
void GameObject::late_update(float time) {
	for (auto comp : components_)
		if (comp != nullptr && comp->isActive())comp->late_update(this, time);
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::addComponent(Component* comp) {
	components_.push_back(comp);
}
void GameObject::addComponent(std::list<Component*> comps) {
	for (auto comp : comps) components_.push_back(comp);
}

void GameObject::delComponent(Component* comp) {
	auto it = std::find(components_.begin(), components_.end(), comp);
	if (it != components_.end()) components_.erase(it);
}
void GameObject::delComponent(std::list<Component*> comps) {
	for (auto comp : comps) {
		auto it = std::find(components_.begin(), components_.end(), comp);
		if (it != components_.end()) components_.erase(it);
	}
}

void GameObject::clearComponents() {
	components_.clear();
}
