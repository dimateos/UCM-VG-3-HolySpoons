//Nap_Time_Studios
#ifndef COLLISION_LISTENER_H_
#define COLLISION_LISTENER_H_

#include "Identifiable.h"
#include "Transforms.h"

class CollisionListener; //fowarding

//holds a collisionListeners list
class CollisionListenersHolder
{
public:
	inline CollisionListenersHolder() : collisionListeners_() {};

	inline void addCollisionListener(CollisionListener* cl) { collisionListeners_.push_back(cl); }
	inline std::list<CollisionListener*>* getCollisionListeners() { return &collisionListeners_; }

protected:
	std::list<CollisionListener*> collisionListeners_;	// optimize collisions
};

//listens collisions
class CollisionListener
{
public:
	inline CollisionListener() {}
	inline CollisionListener(CollisionListenersHolder* owner) { owner->addCollisionListener(this); };
	inline ~CollisionListener() {};

	virtual void onCollision(ID* other) = 0;

	inline bool isListening() { return collisionListening_; }
	inline bool toggleListening() { collisionListening_ = !collisionListening_; }
	inline void setListening(bool l) { collisionListening_ = l; }
	inline void setCollisionListener(CollisionListenersHolder* owner) { owner->addCollisionListener(this); }

private:
	bool collisionListening_;
};

//composed by a GO trans, ID and collision listeners
class nap_userData
{
public:
	inline nap_userData(nap_transform * trans, std::list<CollisionListener*>* cls, ID* id)
		: trans_(trans), collisionListeners_(cls), id_(id) {};

	nap_transform * trans_;
	std::list<CollisionListener*>* collisionListeners_;
	ID* id_;
};

#endif /* COLLISION_LISTENER_H_ */
