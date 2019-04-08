//Nap_Time_Studios
#ifndef INITIABLE_H_
#define INITIABLE_H_

// class to allow setUp/setDown and kill/notKilled
class Initiable
{
public:
	inline Initiable() : inited_(false), killed_(false) {};
	inline Initiable(bool i): inited_(i), killed_(false) {};
	inline virtual ~Initiable() {};

	virtual void setUp() = 0;
	virtual void lateSetUp() {};
	//virtual void setDown() {}; //maybe not pure virtual

	inline bool isInited() const { return inited_; }
	inline void setInited(bool b = true) { inited_ = b; }

	inline bool isKilled() const { return killed_; }
	inline void setKilled(bool b = true) { killed_ = b; }

protected:
	bool inited_, killed_;
};

#endif /* INITIABLE_H_ */

