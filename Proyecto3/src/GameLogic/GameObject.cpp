#include "GameObject.h"
#include "Component.h"

#include "Transforms.h"

GameObject::GameObject(nap_json const & cfg)
	: Activable(FINDnRETURN(cfg, "active", bool, true)), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_() {}
GameObject::GameObject(nap_json const & cfg, std::list<Component*> comps)
	: Activable(FINDnRETURN(cfg, "active", bool, true)), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_(comps) {}

GameObject::GameObject(GameObject * o) :
	Activable(o->isActive()), Identifiable(o->cfg_["id"]), Initiable(o->isInited()), cfg_(o->cfg_), components_(o->getComponents()) {}

GameObject::~GameObject() {
	setDown();
}

void GameObject::setUp() {
	if (isInited()) return;
	setInited();

	//set transform
	if (FIND(cfg_, "pos")) setPosition(nap_vector3(cfg_["pos"]));
	if (FIND(cfg_, "ori")) setOrientation(nap_quat(cfg_["ori"]));
	if (FIND(cfg_, "scale")) setScale(nap_vector3(cfg_["scale"]));

	//init components
	for (auto comp : components_) if (comp != nullptr) comp->setUp();
}

void GameObject::lateSetUp() {
	for (Component* comp : components_) if (comp != nullptr) comp->lateSetUp();

	//config active/inactive
	configActive();
}

void GameObject::setDown() {
	for (auto comp : components_) {
		if (comp != nullptr) delete comp;
	}

	components_.clear();
}

//inactive components will be set to active even if they were not active tho
void GameObject::configActive() {
	for (Component* comp : components_) if (comp != nullptr) comp->setActive(active_);
}

///////////////////////////////////////////////////////////////////////////////

bool GameObject::handleEvents(const SDL_Event evt) {
	bool handled = false;

	auto it = components_.begin();
	while (!handled && it != components_.end()) {
		if ((*it)->isActive()) handled = (*it)->handleEvents(this, evt);
		if (!handled) it++;
	}

	return handled;
}

void GameObject::update(double time) {
	for (auto comp : components_)
		if (comp != nullptr && comp->isActive())comp->update(this, time);
}
void GameObject::late_update(double time) {
	for (auto comp : components_)
		if (comp != nullptr && comp->isActive())comp->late_update(this, time);
}

///////////////////////////////////////////////////////////////////////////////

void GameObject::addComponent(Component* comp) {
	components_.push_back(comp);
	if (isInited()) {
		comp->setUp();
		comp->lateSetUp();
	}
}
void GameObject::addComponent(std::list<Component*> comps) {
	for (auto comp : comps) {
		components_.push_back(comp);
		if (isInited()) {
			comp->setUp();
			comp->lateSetUp();
		}
	}
}

void GameObject::delComponent(std::string name) {
	for (auto it = components_.begin(); it != components_.end(); it++) {
		if ((*it)->id().name_ == name) {
			components_.erase(it);
			break;
		}
	}
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

Component* GameObject::getComponent(std::string name) {
	Component* found = nullptr;

	for (auto comp : components_) {
		if (comp->id().name_ == name) {
			found = comp;
			break;
		}
	}
	return found;
}
std::list<Component*> GameObject::getComponents() {
	return components_;
}
void GameObject::clearComponents() {
	components_.clear();
}
