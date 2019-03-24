//Nap_Time_Studios
#ifndef IDENTIFIABLE_H_
#define IDENTIFIABLE_H_

#include <string>

// instead of just a string the id could also have a unique serial number or stuff like that
struct ID
{
	inline ID (std::string name) : name_(name) {};
	std::string name_;

	inline bool equal(std::string name) const { return name_ == name; }
};

// class to serve as identification, could be expanded to gropups or etc
class Identifiable
{
public:
	inline Identifiable() : id_("undefined") {};
	inline Identifiable(std::string id): id_(id == "" ? "undefined" : id) {};
	inline ~Identifiable() {};

	//identification (atm cannot change it but maybe in the future)
	ID id() const { return id_; }

private:
	ID id_;
};

#endif /* IDENTIFIABLE_H_ */

