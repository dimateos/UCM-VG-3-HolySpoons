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

JsonReader::JsonReader() { preloadPrefabs(); preloadMapTypes(); }
JsonReader::~JsonReader() {
	//delete the GOStructs
	for (auto itr = prefabs.begin(); itr != prefabs.end(); itr++) {
		delete (itr->second);
	}
	prefabs.clear();
	mapTypes.clear();
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

GOStruct JsonReader::getPrefab(string pref, bool & success) {
	if (prefabs.count(pref) > 0) {
		success = true;
		return *prefabs[pref];
	}
	else {
		LogSystem::Log("El prefab " + pref + "no fue encontrado... abortando parseo del GO", LogSystem::JSON);
		success = false;
		return GOStruct();
	}
}

///////////////////////////////////////////////////////////////////////////////

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

//update all nested json objects
void JsonReader::deepUpdateJson(nap_json & j, nap_json const & updater) {
	//json.update(updater); //not recursive!!!!!!
	nap_json::json_pointer ptr;
	deepUpdateJson_rec(j, updater, ptr); //recursive part uses a json pointer
}
void JsonReader::deepUpdateJson_rec(nap_json & j, nap_json const & updater, nap_json::json_pointer & ptr) {
	//iterate objects layer by layer
	for (auto it = updater.begin(); it != updater.end(); ++it) {
		if (it->is_structured()) {
			ptr.push_back(it.key());			//keep track of the pointer
			deepUpdateJson_rec(j, *it, ptr);	//nested object
			ptr.pop_back();						//back from nested
		}
		else {
			ptr.push_back(it.key());	//add key to ptr
			j[ptr] = it.value();		//update the value of j
			ptr.pop_back();				//back ptr from the key
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

// returns a Scene_type with all the information read
// from "level.json" (GameObjects) and "level.txt" (map/tiles)
SceneStruct JsonReader::ReadLevel(string level) {
	LogSystem::Log("Reading level " + level + " ...", LogSystem::JSON);
	size_t n = 0;

	SceneStruct scene;

	//read the map/tiles ("level.txt") and dump all the GOs
	scene.gameObjects = ReadMap(level);

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
				bool success;
				GOStruct prefab_go = getPrefab(cfg["id"]["type"], success);
				if (!success) continue;

				//update all the prefab values with raw GO
				//using recursive deep update
				deepUpdateJson(prefab_go.go_cfg, go.go_cfg);
				deepUpdateJson(prefab_go.components_cfg, go.components_cfg);
				go = prefab_go;
			}
		}

		scene.gameObjects.push_back(go); //add the go to the scene
		n++;
	}

	file.close();
	LogSystem::Log("Leidos " + to_string(n) + " Gameobjects con exito", LogSystem::JSON);
	return scene;
}

///////////////////////////////////////////////////////////////////////////////

void JsonReader::preloadMapTypes() {
	LogSystem::Log("Reading map types file" + routeMapsTypes + " ...", LogSystem::JSON);
	size_t n = 0;

	//check if openned correctly
	ifstream file(routeMapsTypes);
	if (!file.is_open()) {
		LogSystem::Log("El archivo " + routeMapsTypes + " no fue encontrado... abortando parseo", LogSystem::JSON);
		return;
	}

	//read the pairs untill the end
	char c; string s;
	while (file >> c >> s) {

		//check the prefab exists and save / throw error and continue
		if (prefabs.count(s) == 0) {
			LogSystem::Log("El prefab " + s + " no fue encontrado... abortando parseo del mapType", LogSystem::JSON);
			continue;
		}

		mapTypes[c] = s;
		n++;

	}

	file.close();
	LogSystem::Log("Leidos " + to_string(n) + " tipos con exito", LogSystem::JSON);
}

// it adds to the scene the GameObjects (tiles) that will form the floor
GOType JsonReader::ReadMap(string level) {
	LogSystem::Log("Reading map " + level + " ...", LogSystem::JSON);
	size_t n = 0;

	GOType map;

	//check if openned correctly
	ifstream file(routeLevel + level + ".txt");
	if (!file.is_open()) {
		LogSystem::Log("El mapa " + routeLevel + level + ".txt no fue encontrado... abortando parseo", LogSystem::JSON);
		return map;
	}

	//read rows and cols
	int r, c;
	file >> r >> c;

	//read tiles
	for (int i = 0; i < r; i++) {
		string row; file >> row;

		for (int j = 0; j < c; j++) {
			// the map is composed of chars
			// the chars are mapped to the prefab name on mapTypes
			// mapTypes is preloaded from file "routeMapsTypes" in preloadMapTypes()

			//check if mapType exists
			if (mapTypes.count(row[j]) == 0) {
				string s = " "; s[0] = row[j];
				LogSystem::Log("El mapType '" + s + "' no fue encontrado... abortando parseo de tile", LogSystem::JSON);
				continue;
			}

			GOStruct go = *prefabs[mapTypes[row[j]]];

			//temporal way of adding suffix to entity name. Ogre dosent allow same name. //FIXED
			//go.components_cfg["tile_rend"]["shape"]["entity_name"] = "tile_" + to_string(n);

			//configue its position and push it
			setTilePosition(r, c, i, j, go);
			map.push_back(go);
			n++;
		}
	}

	file.close();
	LogSystem::Log("Leidos " + to_string(n) + " Gameobjects con exito", LogSystem::JSON);
	return map;
}

#include "Transforms.h"
// set the physical position of the tile parsing its logic position (i, j)
void JsonReader::setTilePosition(int r, int c, int i, int j, GOStruct & go) {
	const float POSITION_FACTOR_C = 5.0f; //at some point moved to and external cfg file
	const float POSITION_FACTOR_R = 5.0f;

	float x, y, z;
	x = -((c / 2.0f) * POSITION_FACTOR_C - (POSITION_FACTOR_C / 2.0f)) + j * POSITION_FACTOR_C;
	y = -5.0;
	z = (r / 2.0f) * POSITION_FACTOR_R - (POSITION_FACTOR_R / 2.0f) + i * POSITION_FACTOR_R;

	go.go_cfg["pos"] = nap_vector3(x, y, z).json();
}