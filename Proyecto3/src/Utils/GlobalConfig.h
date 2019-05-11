#ifndef GLOBALCFG_H_
#define GLOBALCFG_H_

#include "jsonCore.h"
#include <string>
#include <map>
using namespace std;

typedef signed __int32 SDL_Keycode; //fowarded

#define iniCFG_path "./user/config.json"

// loads and holds some values for all the game
class GlobalCFG
{
public:
	static std::string name;
	static std::map<std::string, std::string> paths, strings;
	static std::map<std::string, bool> flags;
	static std::map<std::string, float> floats;
	static std::map<std::string, SDL_Keycode> keys;

	static void loadIniCFG();

private:
	static nap_json loadJson(std::string path);

	template<typename T>
	static void readVals(nap_json & list, std::map<std::string, T>& map);
	static void readKeys(nap_json & list);
};

#endif /* GLOBALCFG_H_ */