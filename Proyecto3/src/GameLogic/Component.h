#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "Entity.h"

class Component { // clase de la que heredaran todos los componentes
public:
	Component() {}
	virtual ~Component() {}

	// se llaman desde la entidad
	virtual void update(Entity* ent, float time) = 0;
	virtual void handleInput(Entity* ent, float time, const Event& evt) = 0;

	/*// se llaman desde Ogre y Physx
	virtual void render(Entity* ent, float time) = 0;
	virtual void physics(Entity* ent, float time) = 0;
	*/

	virtual void setUp() = 0;   // introduce al componente en Ogre/physx
	virtual void setDown() = 0; // quita al componente de Ogre/physx
};

#endif /* COMPONENT_H_ */
