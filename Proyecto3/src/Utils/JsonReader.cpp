#include "JsonReader.h"
#include "LogSystem.h"

#include <fstream>

JsonReader* JsonReader::instance_ = nullptr;
JsonReader* JsonReader::getSingleton() {
	if (instance_ == nullptr) {
		instance_ = new JsonReader();
	}

	return instance_;
}

void JsonReader::shutdownSingleton() {
	if (instance_ == nullptr) return;

	delete instance_;
	instance_ = nullptr;
}

JsonReader::JsonReader() { preloadPrefabs(); }
JsonReader::~JsonReader() {
	//delete the GOStructs
	for (auto itr = prefabs.begin(); itr != prefabs.end(); itr++) {
		delete (itr->second);
	}
	prefabs.clear();
}

///////////////////////////////////////////////////////////////////////////////

#include "dirent.h"
//preload all the prefabs in folder "routePrefabs"
void JsonReader::preloadPrefabs() {
	LogSystem::Log("Precargando prefabs...", LogSystem::JSON);
	size_t n = 0;

	//read all directory files using dirent.h as filesystem.h is c++ 17
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(routePrefabs.c_str())) == NULL) {
		LogSystem::Log(routePrefabs + " no fue encontrado... abortando parseo prefabs", LogSystem::JSON);
		return;
	}

	//ignore . and ..
	readdir(dir); readdir(dir);

	//parse one by one
	while ((ent = readdir(dir)) != NULL) {
		//open the file
		string s = ent->d_name; ifstream file(routePrefabs + s);
		if (!file.is_open()) {
			LogSystem::Log("El archivo " + s + " no se pudo abrir... ignorando", LogSystem::JSON);
			continue;
		}

		//parse the json
		nap_json j;
		file >> j;
		n++;

		//json can have just one prefab or a list
		if (j.find("Prefabs") != j.end()) { //list of prefabs
			for (size_t i = 0; i < j["Prefabs"].size(); i++) {
				//parse and add it to the map
				GOStruct* go = readGO(j["Prefabs"][i]);
				prefabs[go->go_cfg["id"]["type"]] = go;
			}
		}
		else { //just one prefab
			//parse and add it to the map
			GOStruct* go = readGO(j);
			prefabs[go->go_cfg["id"]["type"]] = go;
		}

		file.close();
	}

	closedir(dir);
	LogSystem::Log("Precargados " + to_string(n) + " archivos de prefabs con exito", LogSystem::JSON);
}

GOStruct* JsonReader::readGO(nap_json const & cfg) {
	auto go = new GOStruct();
	go->go_cfg = cfg;

	//existance of comps
	if (cfg.find("Components") != cfg.end()) {
		go->go_cfg.erase("Components");
		go->components_cfg = cfg["Components"];
	}

	return go;
}

///////////////////////////////////////////////////////////////////////////////

// returns a Scene_type with all the information read
// from "level.json" (GameObjects) and "level.txt" (map/tiles)
SceneStruct JsonReader::ReadLevel(string level) {
	LogSystem::Log("Reading level "+ level + " ...", LogSystem::JSON);
	size_t n = 0;

	SceneStruct scene;
	//ReadMap(level); // it reads the map/tiles ("level.txt"; it will be an amount of GameObjects)

	//read "level.json"
	ifstream file(routeLevel + level + ".json");
	if (!file.is_open()) {
		LogSystem::Log("El archivo " + routeLevel + level + ".json no fue encontrado... abortando parseo", LogSystem::JSON);
		return scene;
	}

	//parse it
	nap_json j;
	file >> j;

	//read the scene name
	if (j.find("stateID_") != j.end()) {
		string s = j["stateID_"];
		scene.SceneName = s;
	}
	else scene.SceneName = "undefined";

	//read the gameobjects
	if (j.find("GameObjects") == j.end() || j["GameObjects"].size() == 0) {
		LogSystem::Log("El archivo " + routeLevel + level + ".json no tiene GOs...", LogSystem::JSON);
		return scene;
	}

	//one by one
	for (size_t i = 0; i < j["GameObjects"].size(); i++) {
		nap_json cfg = j["GameObjects"][i];

		//raw read the GO
		GOStruct go = *readGO(cfg);

		//check if the object has id
		if (cfg.find("id") != cfg.end()) {
			//check if the object uses a prefab as base
			if (cfg["id"].find("type") != cfg["id"].end()) {

				//check the prefab exists and load / throw error and continue
				string type = cfg["id"]["type"];
				if (prefabs.find(type) != prefabs.end()) {
					//update all the prefab values with raw GO
					GOStruct prefab_go = *prefabs[type];
					prefab_go.go_cfg.update(go.go_cfg);
					prefab_go.components_cfg.update(go.components_cfg);
					go = prefab_go;
				}
				else {
					LogSystem::Log("El prefab " + type + "no fue encontrado... abortando parseo del GO", LogSystem::JSON);
					continue;
				}
			}
		}

		n++;
		scene.gameObjects.push_back(go); //add the go to the scene
	}

	file.close();
	LogSystem::Log("Leidos " + to_string(n) + " Gameobjects con exito", LogSystem::JSON);
	return scene;
}

///////////////////////////////////////////////////////////////////////////////

// it adds to the scene the GameObjects (tiles) that will form the floor
void JsonReader::ReadMap(string level) {
	ifstream in(routeLevel + level + ".txt");

	//if (in.is_open()) {
	//	int r, c;
	//	in >> r >> c;
	//	for (int i = 0; i < r; i++) {
	//		string floor; in >> floor;
	//		for (int j = 0; j < c; j++) {
	//			GOType go;
	//			// two types of gamobject: normal floor and falling floor
	//			// each of them read from its prefab
	//			if (floor[j] - 48 == 0) {
	//				preloadPrefabs("Tile", go);
	//				setTilePosition(r, c, i, j, go);
	//				scene.gameObjects.push_back(go);
	//			}
	//			else if (floor[j] - 48 == 1) {
	//				preloadPrefabs("FallingTile", go);
	//				setTilePosition(r, c, i, j, go);
	//				scene.gameObjects.push_back(go);
	//			}
	//		}
	//	}
	//}

	in.close();
}

// set the physical position of the tile parsing its logic position (i, j)
void JsonReader::setTilePosition(int r, int c, int i, int j, GOType& go) {
	const float POSITION_FACTOR_C = 1.1f;
	const float POSITION_FACTOR_R = 1.1f;

	//float x, y, z;
	//x = -((c / 2.0f) * POSITION_FACTOR_C - (POSITION_FACTOR_C / 2.0f)) + j * POSITION_FACTOR_C;
	//y = 0;
	//z = (r / 2.0f) * POSITION_FACTOR_R - (POSITION_FACTOR_R / 2.0f) - i * POSITION_FACTOR_R;
	//try {
	//	if (go.GOParameters.size() >= 3) {
	//		go.GOParameters[0] = to_string(x); go.GOParameters[1] = to_string(y); go.GOParameters[2] = to_string(z);
	//	}
	//	else throw "Tienes que poner al menos tres parametros en GOParameters de cada prefab Tile (Prefabs.json) para su posicion";
	//}
	//catch (const char* e) {
	//	LogSystem::Log(e, LogSystem::JSON);
	//}
}