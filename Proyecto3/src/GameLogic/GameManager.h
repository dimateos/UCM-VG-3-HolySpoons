//Nap_Time_Studios
#ifndef GAME_MANAGER_H_
#define GAME_MANAGER_H_

#include "Component.h"
#include <Timer.h>

class OverlayComponent;
class GameObject;
class HPComponent;
class napTimer;

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
	Ogre::TextAreaOverlayElement* RoundText;
	Ogre::TextAreaOverlayElement* MiniRoundText;

	// timers
	napTimer hitTimer;
	napTimer roundTimer;

	// timer durations
	float hitTime;
	float deathTime;
	float roundTime;

	GameObject* player_;
	HPComponent* playerHP_;

	int score_;
	int enemies_; // enemies to kill in the actual round
	int round_;

	std::string death_state;
	std::string hitMarker_sound;

	void updateUI();
	void nextRound();

public:
	inline GameManager(nap_json const & cfg, GameObject* owner) : 
		Component(cfg, owner), score_(0), enemies_(0), round_(0) {};
	inline virtual ~GameManager() { };
	virtual void setUp();
	virtual void lateSetUp();

	virtual void update(GameObject* o, double time);
	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void receive(Message* msg);

	void resetPlayer();
	inline void addScore(int moreScore) { score_ += moreScore; }
};

#endif /* GAME_MANAGER_H_ */
