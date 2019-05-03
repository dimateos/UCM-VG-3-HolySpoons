#include "GlobalConfig.h"

#include "KeyMapper.h"

#include <fstream>	//files
#include <iostream> //raw debuggin

//maps
std::map<std::string, std::string> GlobalCFG::paths = {};
std::map<std::string, std::string> GlobalCFG::strings = {};
std::map<std::string, bool> GlobalCFG::flags = {};
std::map<std::string, float> GlobalCFG::floats = {};
std::map<std::string, SDL_Keycode> GlobalCFG::keys = {};

void GlobalCFG::loadIniCFG() {
	nap_json j = loadJson(iniCFG_path);

	//base user config set here for easy access
	readKeys(j["controls"]);
	readVals(j["camera"], floats);
	readVals(j["sound"], floats);
	readVals(j["holding"], flags);

	//finally load the global config
	nap_json jCFG = loadJson(j["appConfigPath"]);

	readVals(jCFG["paths"], paths);
	readVals(jCFG["strings"], strings);
	readVals(jCFG["flags"], flags);
	readVals(jCFG["floats"], floats);
}

nap_json GlobalCFG::loadJson(std::string path) {
	nap_json j;
	ifstream file(path);

	//parse it
	if (file.is_open()) file >> j;
	else std::cout << "error opening " << path << std::endl;

	return j;
}

template <typename T>
void GlobalCFG::readVals(nap_json & list, std::map<std::string, T> & map) {
	for (auto & o : list) {
		string mapKey = o.items().begin().key();
		T v = o.items().begin().value();
		map[mapKey] = v;
	}
}

void GlobalCFG::readKeys(nap_json & list) {
	for (auto & o : list) {
		string mapKey = o.items().begin().key();
		string k = o.items().begin().value();
		keys[mapKey] = KeyMapper::getKeycode(k);
	}
}
