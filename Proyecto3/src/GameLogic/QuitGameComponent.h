#ifndef QUITGAMECOMPONENT_H_
#define QUITGAMECOMPONENT_H_

#include "Component.h"

class QuitGameComponent: public Component
{
private:
	void clearStates();

public:
	virtual ~QuitGameComponent() {};
	QuitGameComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	bool handleEvents(GameObject * o, const SDL_Event & evt);
	void receive(Message * msg);
};

#endif //QUITGAMECOMPONENT_H_