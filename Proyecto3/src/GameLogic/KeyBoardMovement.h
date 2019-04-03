#ifndef KEYBOARDMOVEMENT_H_
#define KEYBOARDMOVEMENT_H_

#include "Component.h"

class KeyBoardMovement : public Component
{
private:
	SDL_Keycode forward_;   // W
	SDL_Keycode left_;      // A
	SDL_Keycode backward_;  // S
	SDL_Keycode right_;     // D
	SDL_Keycode run_;       // Lft. Shift
	SDL_Keycode jump_;      // Space

	// velocities
	float walkVel_;         // while walking
	float runVel_;          // while running
	float vel_;             // actual velocity
	float jumpForce_;

	bool jumped = false;

public:
	KeyBoardMovement(nap_json const & cfg, GameObject* owner);

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void setUp();

	virtual ~KeyBoardMovement();
};

#endif /* KEYBOARDMOVEMENT_H_ */

