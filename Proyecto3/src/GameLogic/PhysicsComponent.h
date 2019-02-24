#ifndef PHYSICSCOMPONENT_H_
#define PHYSICSCOMPONENT_H_

class Entity;

class PhysicsComponent {
public:
	PhysicsComponent() {}
	virtual ~PhysicsComponent() {}
	virtual void update(Entity* ent, float time) = 0;
};

#endif /* PHYSICSCOMPONENT_H_ */
