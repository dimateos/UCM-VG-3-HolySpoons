#ifndef UPTODATE_H_
#define UPTODATE_H_

class upToDate
{
public:
	enum props { scale, pos, ori }; //outside
	enum systems { PHYS, REND };	//inside

	inline void setUpToDate(systems sys, bool b = true) {
		if (sys == PHYS) phys = b;
		else rend = b;
	}
	inline void setUpToDate(bool b = true) {
		phys = b;
		rend = b;
	}

	inline bool getUpToDate_obj(systems sys) {
		if (sys == PHYS) return phys;
		else return rend;
	}
	inline bool getUpToDate_obj() {
		return phys && rend;
	}

private:
	bool phys = true, rend = false;
};

#endif /* UPTODATE_H_ */

