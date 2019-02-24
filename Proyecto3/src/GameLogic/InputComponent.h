#ifndef INPUTCOMPONENT_H_
#define INPUTCOMPONENT_H_

class Entity;

class InputComponent {
public:
	InputComponent() {}
	virtual ~InputComponent() {}
	virtual void handleInput(Entity* ent, float time/*, const SDL_Event& event*/) = 0;
};

#endif /* INPUTCOMPONENT_H_ */
