//Nap_Time_Studios
#ifndef ACTIVABLE_H_
#define ACTIVABLE_H_

#include "LogSystem.h"
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
	void setActive(bool active = true, bool instant = false) {
		active_ = active;
		if (instant) configActive();
		else changedActive_.push_back(this);
	}

	//list of objects that changed active, all will be config active before update
	static std::list<Activable*> changedActive_;
	static void do_ConfigActive() {
		//int s = Activable::changedActive_.size();
		//if (s == 0) return;
		//LogSystem::Log("changedActive: ", s);

		for (Activable* & a : Activable::changedActive_) a->configActive();
		Activable::changedActive_.clear();
	}

	//override to set up/down required stuff
	virtual void configActive() {}

protected:
	bool active_;
};

#endif /* ACTIVABLE_H_ */

