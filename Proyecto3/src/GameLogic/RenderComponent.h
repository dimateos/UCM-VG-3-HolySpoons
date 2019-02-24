#ifndef RENDERCOMPONENT_H_
#define RENDERCOMPONENT_H_

class Entity;

class RenderComponent {
public:
	RenderComponent() {}
	virtual ~RenderComponent() {}
	virtual void render(Entity* ent, float time) = 0;
};

#endif /* RENDERCOMPONENT_H_ */
