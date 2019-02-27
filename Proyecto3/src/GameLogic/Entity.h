#ifndef ENTITY_H_
#define ENTITY_H_

#include <vector>

class Component;

using namespace std;

//placeholders
enum Event
{
	UNDEFINED, GAME_QUIT
};

class Entity     // clase que acepta componentes (contenedor vacio esperando funcionalidad)
{
private:
	bool active;                    // indica si la entidad esta activa o no
	vector<Component*> Components_; // vector de componentes

public:
	Entity();

	inline bool isActive() { return active; }
	inline bool toggleActive() { active = !active; }
	inline void setActive(bool active_) { active = active_; }

	virtual void handleEvents(float time, const Event evt);
	virtual void update(float time);

	// añade un componente
	virtual void addComponent(Component* component);

	// quita un componente
	virtual void delComponent(Component* component);

	virtual ~Entity();
};

#endif /* ENTITY_H_ */

