#ifndef COMPONENT_H_
#define COMPONENT_H_

#include <vector>
#include "GameObject.h"

using namespace std;

// every type of component will inherit from this class
class Component : public Activable
{
public:
	Component() : Activable() {};
	virtual ~Component() {};

	// called from gameObjects themselves
	virtual void update(GameObject* ent, float time) = 0;
	virtual bool handleEvents(GameObject* ent, const SDL_Event& evt) = 0;

	// each component will define this method depending on its needs
	virtual void loadParameters(std::vector<string>parameters) = 0;

protected:
	virtual void setUp() = 0;   // set up in Ogre/physx
	virtual void setDown() = 0; // set down from Ogre/physx
};

#endif /* COMPONENT_H_ */
