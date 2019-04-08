//Nap_Time_Studios
#ifndef ACTIVABLE_H_
#define ACTIVABLE_H_

// class to allow active/desactive objects
class Activable
{
public:
	inline Activable() : active_(true) {};
	inline Activable(bool act): active_(act) {};
	inline virtual ~Activable() {};

	inline bool isActive() { return active_; }
	inline bool toggleActive() { active_ = !active_; }
	inline void setActive(bool active = true) { active_ = active; }

protected:
	bool active_;
};

#endif /* ACTIVABLE_H_ */

