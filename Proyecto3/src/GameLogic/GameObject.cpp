#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(nap_json const & cfg)
	: Emitter(), Activable(), Identifiable(cfg.find("name") != cfg.end() ? cfg["name"] : ""), components_() {
	setUp(cfg);
}
GameObject::GameObject(nap_json const & cfg, std::list<Component*> comps)
	: Emitter(), Activable(), Identifiable(cfg.find("name") != cfg.end() ? cfg["name"] : ""), components_(comps) {
	setUp(cfg);
}
GameObject::GameObject(nap_json const & cfg, std::list<Component*> comps, std::list<Listener*> lis)
	: Emitter(lis), Activable(), Identifiable(cfg.find("name") != cfg.end() ? cfg["name"] : ""), components_(comps) {
	setUp(cfg);
}

GameObject::~GameObject() {
	for (auto comp : components_) {
		if (comp != nullptr)delete comp;
	}

	components_.clear();
}

void GameObject::setUp(nap_json const & cfg) {
	//set transform
	if (cfg.find("pos") != cfg.end()) setPosition(nap_vector3(cfg["pos"]));
	if (cfg.find("ori") != cfg.end()) setOrientation(nap_quat(cfg["ori"]));

	//send user ptr to physics component
	send(&Msg_PX_userPtr(getTransPtr()));
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

void GameObject::delComponent(std::string name) {
	for (auto it = components_.begin(); it != components_.end(); it++) {
		if ((*it)->id().equal(name)) {
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
		if (comp->id().equal(name)) {
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