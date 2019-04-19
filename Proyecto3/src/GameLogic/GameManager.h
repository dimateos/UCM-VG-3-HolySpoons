//Nap_Time_Studios
#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Component.h"

// the gameManager object would have this component
// this component manages the logic of the game and controls the HUD
// with the other OverlayComponents of its object
class GameManager : public Component
{
public:
	inline GameManager() { };
	inline GameManager(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~GameManager() { };
	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
};

#endif /* GAME_MANAGER_H_ */
