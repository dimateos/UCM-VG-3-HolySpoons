#include "GameObject.h"

//fowarded
#include <Transforms.h>
#include <SDL_events.h>	//events
#include "Component.h"

GameObject::GameObject(nap_json const & cfg)
	: Activable(FINDnRETURN(cfg, activeString, bool, true)), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_() {}
GameObject::GameObject(nap_json const & cfg, std::list<Component*> comps)
	: Activable(FINDnRETURN(cfg, activeString, bool, true)), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_(comps) {}

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
}

void GameObject::setDown() {
	for (auto comp : components_) {
		if (comp != nullptr) delete comp;
	}

	components_.clear();
}

//inactive components will be set to active even if they were not active tho
void GameObject::configActive() {
	for (Component* comp : components_) if (comp != nullptr) comp->setActive(active_, true);
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

///////////////////////////////////////////////////////////////////////////////

upToDate* GameObject::getUpToDate_obj(upToDate::props prop) {
	if (prop == upToDate::pos) return &trans_.upToDate_pos;
	else if (prop == upToDate::ori) return &trans_.upToDate_ori;
	return &scale_.upToDate_scale;
}

bool GameObject::getUpToDate(upToDate::props prop, upToDate::systems sys) {
	return getUpToDate_obj(prop)->getUpToDate_obj(sys);
}
void GameObject::setUpToDate(upToDate::props prop, upToDate::systems sys, bool b) {
	getUpToDate_obj(prop)->setUpToDate(sys, b);
}

void GameObject::setUpToDate_prop(upToDate::props prop, bool b) {
	auto p = getUpToDate_obj(prop);
	p->setUpToDate(upToDate::PHYS, b);
	p->setUpToDate(upToDate::REND, b);
}

void GameObject::setUpToDate_trans(bool b) {
	setUpToDate_prop(upToDate::pos, b);
	setUpToDate_prop(upToDate::ori, b);
}
void GameObject::setUpToDate_trans(upToDate::systems sys, bool b) {
	setUpToDate(upToDate::pos, sys, b);
	setUpToDate(upToDate::ori, sys, b);
}
bool GameObject::getUpToDate_trans(upToDate::systems sys) {
	return getUpToDate(upToDate::pos, sys)
		&& getUpToDate(upToDate::ori, sys);
}

