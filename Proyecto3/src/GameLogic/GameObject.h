#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <list>
#include <SDL_events.h> //events
using namespace std;


#include <OgreSceneNode.h> //tmp for vector3
//struct for the gameObject transform
enum updateState { upToDate, pxUpdated, userUpdated };
struct nap_transform
{
	updateState updateState_ = userUpdated;
	Ogre::Vector3 p_; Ogre::Quaternion q_;
};

#include "Activable.h"
#include "Identifiable.h"
class Component;

// this class accepts components (empty container waiting for functionality)
class GameObject : public Activable, public Identifiable
{
public:
	GameObject();
	GameObject(std::list<Component*>);
	virtual ~GameObject();

	//MAIN methods
	virtual bool handleEvents(const SDL_Event evt);
	virtual void update(float time);
	virtual void late_update(float time);

	//components operations
	void addComponent(Component* component);
	void addComponent(std::list<Component*> comps);
	void delComponent(std::string name);
	void delComponent(Component* component);
	void delComponent(std::list<Component*> comps);
	Component* getComponent(std::string name);
	std::list<Component*> getComponents();
	void clearComponents();

	//transform operations (set's activates the flags)
	inline void setPosition(Ogre::Vector3 p) { trans.p_ = p; trans.updateState_ = userUpdated; }
	inline Ogre::Vector3 getPosition() const { return trans.p_; }
	inline void setOrientation(Ogre::Quaternion q) { trans.q_ = q; trans.updateState_ = userUpdated; }
	inline Ogre::Quaternion getOrientation() const { return trans.q_; }
	//updateState flag
	inline void setUpdateState(updateState s) { trans.updateState_ = s; }
	inline updateState getUpdateState() const { return trans.updateState_; }
	//to keep track of the transform (Physx, IA, etc)
	inline nap_transform* getTransPtr() { return &trans; }

private:
	nap_transform trans;
	std::list<Component*> components_; // component list
};

#endif /* GAMEOBJECT_H_ */

