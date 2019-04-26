//Nap_Time_Studios
#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Component.h"

class OverlayComponent;
class GameObject;
class HPComponent;

namespace Ogre {
	class TextAreaOverlayElement;
}

// the gameManager object would have this component
// this component manages the logic of the game and controls the HUD
// with the other OverlayComponents of its object
class GameManager : public Component
{
private:
	OverlayComponent* overlayComp;
	Ogre::TextAreaOverlayElement* HPText;
	Ogre::TextAreaOverlayElement* ScoreText;

	GameObject* player_;
	HPComponent* playerHP_;

	int score;

	void updateUI();

public:
	inline GameManager() { };
	inline GameManager(nap_json const & cfg, GameObject* owner) : Component(cfg, owner), score(0) {};
	inline virtual ~GameManager() { };
	virtual void setUp();

	virtual void update(GameObject* o, double time);
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void receive(Message* msg);

	void resetPlayer();
	inline void addScore(int moreScore) { score += moreScore; }
};

#endif /* GAME_MANAGER_H_ */
