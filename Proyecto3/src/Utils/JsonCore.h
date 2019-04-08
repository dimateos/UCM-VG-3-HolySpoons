#ifndef JSONCORE_H_
#define JSONCORE_H_

//core library
#include "json.hpp"
using nap_json = nlohmann::json;

//some macros
#define FIND(j, s) j.find(s) != j.end()
#define FINDnRETURN(j, s, type, def) FIND(j, s) ? type(j[s]) : def
#define FINDnRETURN_s(j, s, def) FIND(j, s) ? j[s] : def

#endif /* JSONCORE_H_ */

