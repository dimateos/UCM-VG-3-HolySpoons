//Nap_Time_Studios
#ifndef IDENTIFIABLE_H_
#define IDENTIFIABLE_H_

#include "json.hpp"
using nap_json = nlohmann::json;

#define undef "undefined"

// instead of just a string the id could also have a unique serial number or stuff like that
struct ID
{
	inline ID() : name_(undef), type_(undef), group_(undef) {};
	inline ID(std::string name) : name_(name), type_(undef), group_(undef) {};
	inline ID(std::string name, std::string type) : name_(name), type_(type), group_(undef) {};
	inline ID(std::string name, std::string type, std::string group) : name_(name), type_(type), group_(group) {};

	std::string name_, type_, group_;

	inline bool equal(std::string name) const { return name_ == name; }
};

// class to serve as identification, could be expanded to gropups or etc
class Identifiable
{
public:
	inline Identifiable() : id_() {};
	inline Identifiable(std::string name) : id_(name) {};
	inline Identifiable(std::string name, std::string type) : id_(name, type) {};
	inline Identifiable(std::string name, std::string type, std::string group) : id_(name, type, group) {};
	inline Identifiable(nap_json const & cfg) : id_(
		cfg.find("name") != cfg.end() ? (cfg["name"] != "" ? cfg["name"] : undef) : undef,
		cfg.find("type") != cfg.end() ? (cfg["type"] != "" ? cfg["type"] : undef) : undef,
		cfg.find("group") != cfg.end() ? (cfg["group"] != "" ? cfg["group"] : undef) : undef
	) {};

	inline virtual ~Identifiable() {};

	//identification (atm cannot change it but maybe in the future)
	ID id() const { return id_; }

protected:
	ID id_;
};

#endif /* IDENTIFIABLE_H_ */

