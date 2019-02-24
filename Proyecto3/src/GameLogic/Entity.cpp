#include "Entity.h"

Entity::Entity() :active(true), inputComp_(), physicsComp_(), renderComp_()
{
}

//-------------------ACTUALIZAR COMPONENTES----------------------

void Entity::handleInput(float time/*, const SDL_Event& event*/) {
	if (active) 
	{
		for (auto ic : inputComp_) 
			ic->handleInput(this, time/*, event*/);
	}
}

void Entity::update(float time) {
	if (active)
	{
		for (auto pc : physicsComp_)
			pc->update(this, time);
	}
}

void Entity::render(float time) {
	if (active)
	{
		for (auto rc : renderComp_)
			rc->render(this, time);
	}
}

//-----------------AÑADIR COMPONENTES----------------------

void Entity::addInputComponent(InputComponent* ic) {
	inputComp_.push_back(ic);
}

void Entity::addPhysicsComponent(PhysicsComponent* pc) {
	physicsComp_.push_back(pc);
}

void Entity::addRenderComponent(RenderComponent* rc) {
	renderComp_.push_back(rc);
}

//------------------BORRAR COMPONENTES----------------------

void Entity::delInputComponent(InputComponent* ic) {
	std::vector<InputComponent*>::iterator position = std::find(
		inputComp_.begin(), inputComp_.end(), ic);

	if (position != inputComp_.end())
		inputComp_.erase(position);
}

void Entity::delPhysicsComponent(PhysicsComponent* pc) {
	std::vector<PhysicsComponent*>::iterator position = std::find(
		physicsComp_.begin(), physicsComp_.end(), pc);

	if (position != physicsComp_.end())
		physicsComp_.erase(position);
}

void Entity::delRenderComponent(RenderComponent* rc) {
	std::vector<RenderComponent*>::iterator position = std::find(
		renderComp_.begin(), renderComp_.end(), rc);

	if (position != renderComp_.end())
		renderComp_.erase(position);
}

Entity::~Entity() {
	for (auto ic : inputComp_) {
		if (ic != nullptr) { delete ic; ic = nullptr; delInputComponent(ic); }
	}
	for (auto pc : physicsComp_) {
		if (pc != nullptr) { delete pc; pc = nullptr; delPhysicsComponent(pc); }
	}
	for (auto rc : renderComp_) {
		if (rc != nullptr) { delete rc; rc = nullptr; delRenderComponent(rc); }
	}
}