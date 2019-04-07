#ifndef KEYBOARDMOVEMENT_H_
#define KEYBOARDMOVEMENT_H_

#include "Component.h"
#include <vector>

namespace physx {
	class PxRigidDynamic;
}
class nap_vector3;

class KeyBoardMovement : public Component
{
private:
	SDL_Keycode forward_;   // W
	SDL_Keycode left_;      // A
	SDL_Keycode backward_;  // S
	SDL_Keycode right_;     // D
	SDL_Keycode run_;       // Lft. Shift
	SDL_Keycode jump_;      // Space

	physx::PxRigidDynamic* physBody;

	nap_vector3 velocity;

	// velocities
	float walkVel_;          // while walking
	float runVel_;           // while running
	float vel_;              // actual velocity
	float jumpForce_;

	list<SDL_Keycode> Xaxis; // lista de teclas del eje x
	list<SDL_Keycode> Zaxis; // lista de teclas del eje z

	bool jumped = false;

	void updateVelocity(nap_vector3 orientation);

public:
	KeyBoardMovement(nap_json const & cfg, GameObject* owner);

	virtual bool handleEvents(GameObject* o, const SDL_Event& evt);
	virtual void update(GameObject* o, double time);
	virtual void setUp();

	virtual ~KeyBoardMovement();
};

#endif /* KEYBOARDMOVEMENT_H_ */

