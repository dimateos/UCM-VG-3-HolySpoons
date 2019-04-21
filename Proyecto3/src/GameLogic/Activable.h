//Nap_Time_Studios
#ifndef ACTIVABLE_H_
#define ACTIVABLE_H_

#define activeString "active"

// class to allow active/desactive objects
class Activable
{
public:
	inline Activable() : active_(true) {};
	inline Activable(bool act): active_(act) {};
	inline virtual ~Activable() {};

	inline bool isActive() { return active_; }
	inline bool toggleActive() { setActive(!active_); }
	void setActive(bool active = true) { active_ = active; configActive(); }

protected:
	bool active_;

	//override to set up/down required stuff
	virtual void configActive() {}
};

#endif /* ACTIVABLE_H_ */

