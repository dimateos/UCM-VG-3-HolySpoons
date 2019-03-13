#ifndef ACTIVABLE_H_
#define ACTIVABLE_H_

class Activable
{
public:
	inline Activable() : active(false) {};
	inline Activable(bool act): active(act) {};
	inline ~Activable() {};

	inline bool isActive() { return active; }
	inline bool toggleActive() { active = !active; }
	inline void setActive(bool active_) { active = active_; }

private:
	bool active;
};

#endif /* ACTIVABLE_H_ */

