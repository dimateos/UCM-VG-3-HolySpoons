//Nap_Time_Studios
#ifndef IDENTIFIABLE_H_
#define IDENTIFIABLE_H_

#include "jsonCore.h"
#define undef "undefined"

// instead of just a string the id could also have a unique serial number or stuff like that
class ID
{
public:
	inline ID() : name_(undef), type_(undef), group_(undef), sn_(0) {};
	inline ID(std::string name) : name_(name), type_(undef), group_(undef), sn_(getNewSN()) {};
	inline ID(std::string name, std::string type) : name_(name), type_(type), group_(undef), sn_(getNewSN()) {};
	inline ID(std::string name, std::string type, std::string group) : name_(name), type_(type), group_(group), sn_(getNewSN()) {};

	std::string name_, type_, group_;

	size_t sn_;
	inline std::string sn_string() { return "_" + std::to_string(sn_); }

	//also compares the serial numbers
	inline bool equal(std::string name, size_t sn) const { return name_ == name && sn_ == sn; }
	inline bool equal(ID const & id) const { return equal(id.name_, id.sn_); }

protected:
	//static unique serial number generator for all identifiables
	static size_t getNewSN() {
		static size_t current_last_sn_ = 0; //static initialized once
		return ++current_last_sn_;
	}
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
	ID* idPtr() { return &id_; }

	inline void setId(nap_json const & cfg) {
		id_ = ID(cfg.find("name") != cfg.end() ? (cfg["name"] != "" ? cfg["name"] : undef) : undef,
			cfg.find("type") != cfg.end() ? (cfg["type"] != "" ? cfg["type"] : undef) : undef,
			cfg.find("group") != cfg.end() ? (cfg["group"] != "" ? cfg["group"] : undef) : undef);
	}

protected:
	ID id_;
};

#endif /* IDENTIFIABLE_H_ */

