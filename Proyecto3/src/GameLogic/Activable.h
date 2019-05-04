//Nap_Time_Studios
#ifndef ACTIVABLE_H_
#define ACTIVABLE_H_

#define activeString "active"
#include <list>

// class to allow active/desactive objects
class Activable
{
public:
	inline Activable() : active_(true) {};
	inline Activable(bool act): active_(act) {};
	inline virtual ~Activable() {};

	inline bool isActive() { return active_; }
	inline bool toggleActive() { setActive(!active_); }
	void setActive(bool active = true) { active_ = active; changedActive_.push_back(this); }

	//list of objects that changed active, all will be config active before update
	static std::list<Activable*> changedActive_;

	//override to set up/down required stuff
	virtual void configActive() {}

protected:
	bool active_;
};

#endif /* ACTIVABLE_H_ */

