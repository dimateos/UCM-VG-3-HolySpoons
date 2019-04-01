#include "GameObject.h"
#include "Component.h"

GameObject::GameObject(nap_json const & cfg)
	: Activable(), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_() {
}
GameObject::GameObject(nap_json const & cfg, std::list<Component*> comps)
	: Activable(), Identifiable(cfg["id"]), Initiable(), cfg_(cfg), components_(comps) {
}

GameObject::~GameObject() {
	for (auto comp : components_) {
		if (comp != nullptr)delete comp;
	}

	components_.clear();
}

void GameObject::setUp() {
	if (isInited()) return;
	setInited();

	//set transform
	if (cfg_.find("pos") != cfg_.end()) setPosition(nap_vector3(cfg_["pos"]));
	if (cfg_.find("ori") != cfg_.end()) setOrientation(nap_quat(cfg_["ori"]));

	

	//init components
	for (auto comp : components_) if (comp != nullptr) comp->setUp();

	//send user ptr to physics component (done by component factory)

	//getComponent("Phys")->receive(&Msg_PX_userPtr(getTransPtr()));
}

void GameObject::lateSetUp()
{
	for (Component* comp : components_) if (comp != nullptr) comp->lateSetUp();
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
	if (isInited()) comp->setUp();
}
void GameObject::addComponent(std::list<Component*> comps) {
	for (auto comp : comps) {
		components_.push_back(comp);
		if (isInited()) comp->setUp();
	}
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