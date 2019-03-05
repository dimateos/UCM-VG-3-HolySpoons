#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <vector>
#include <SDL_events.h> //events

class Component;

using namespace std;

class GameObject     // this class accepts components (empty container waiting for functionality)
{
private:
	bool active;                         // it indicates if the entity is active or not
	std::vector<Component*> Components_; // component vector

public:
	GameObject();

	inline bool isActive() { return active; }
	inline bool toggleActive() { active = !active; }
	inline void setActive(bool active_) { active = active_; }

	virtual bool handleEvents(const SDL_Event evt);
	virtual void update(float time);

	// it adds a component
	virtual void addComponent(Component* component);

	// it deletes a component
	virtual void delComponent(Component* component);

	virtual ~GameObject();
};

#endif /* GAMEOBJECT_H_ */

