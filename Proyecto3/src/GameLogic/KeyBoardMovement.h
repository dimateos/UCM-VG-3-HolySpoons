#ifndef KEYBOARDMOVEMENT_H_
#define KEYBOARDMOVEMENT_H_

#include "PhysicsControllerComponent.h"
#include <SDL_events.h>

#include "CollisionListener.h"
#include "Component.h"
class KeyBoardMovement : public Component, public CollisionListener
{
private:
	SDL_Keycode forward_;   // W
	SDL_Keycode left_;      // A
	SDL_Keycode backward_;  // S
	SDL_Keycode right_;     // D
	SDL_Keycode run_;       // Lft. Shift
	SDL_Keycode jumpKey_;      // Space

	PhysicsControllerComponent* controller_comp;
	nap_vector3 velocity;
	bool jumping_ = false, jump_available_ = false;

	// velocities
	float walkVel_;          // while walking
	float runVel_;           // while running
	float vel_;              // actual velocity
	float jumpImpulse_;

	list<SDL_Keycode> Xaxis; // x axis keys
	list<SDL_Keycode> Zaxis; // z axis keys

	// updates the go velocity depending on an orientation
	void updateVelocity(nap_vector3 orientation);
	void jump();

public:
	KeyBoardMovement(nap_json const & cfg, GameObject* owner);
	virtual ~KeyBoardMovement();
	virtual void setUp();
	virtual void lateSetUp();

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void update(GameObject* o, double time);
	virtual void onCollision(ID* other);
};

#endif /* KEYBOARDMOVEMENT_H_ */

