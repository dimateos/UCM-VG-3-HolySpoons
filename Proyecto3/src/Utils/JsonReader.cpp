#include "JsonReader.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "LogSystem.h"

using json = nlohmann::json;
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

JsonReader::JsonReader() { }
JsonReader::~JsonReader() { }

// it returns a Scene_type with all the information read
// from "level.json" (GameObjects) and "level.txt" (map/ground)
Scene_Type* JsonReader::ReadLevel(string level) {

	ReadMap(level); // it reads the map/ground ("level.txt"; it will be an amount of GameObjects)

	// it will read de GameObjects of the level ("level.json")
	ifstream i(routeLevel + level + ".json");
	if (i.is_open()) {
		json j;
		i >> j;

		// reading of the gameobjects
		if (!j["GameObjects"].is_null()) {
			for (int i = 0; i < j["GameObjects"].size(); i++) {
				GOStruct go; // for each gameobject read in the .json

				// we read it from the prefab
				if (!j["GameObjects"][i]["Name"].is_null()) {
					string prefabName = j["GameObjects"][i]["Name"];
					go.GOName = prefabName;
					ReadPrefab(prefabName, go);
				}

				// we read the custom constructor parameters of the go
				if (!j["GameObjects"][i]["GOParameters"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["GOParameters"].size(); x++) {
						string parameter = j["GameObjects"][i]["GOParameters"][x];
						if (parameter != "-")go.GOParameters[x] = parameter;
					}
				}

				// we read the custom parameters of each component
				if (!j["GameObjects"][i]["Custom"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["Custom"].size(); x++) {
						CompType::iterator it; string customName = j["GameObjects"][i]["Custom"][x]["Name"];
						it = findComponent(go.components, customName);

						if (it < go.components.end()) {
							for (int k = 0; k < j["GameObjects"][i]["Custom"][x]["Parameters"].size(); k++) {
								if (j["GameObjects"][i]["Custom"][x]["Parameters"][k] != "-") {
									string parameter = j["GameObjects"][i]["Custom"][x]["Parameters"][k];
									it->compParameters[k] = parameter;
								}
							}
						}
					}
				}

				// we read the components that will be listeners and emitters
				if (!j["GameObjects"][i]["ComponentMessages"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["ComponentMessages"].size(); x++) {
						string emitter = j["GameObjects"][i]["ComponentMessages"][x]["Emitter"];
						string listener = j["GameObjects"][i]["ComponentMessages"][x]["Listener"];
						if (findComponent(go.components, emitter) < go.components.end() && findComponent(go.components, listener) < go.components.end())
							go.compMessages.push_back({ emitter, listener });
					}
				}
				scene.gameObjects.push_back(go); // we push the new gameobject
			}
		}

		// reading of the gameobjects that will be listeners and emitters
		if (!j["GameObjectMessages"].is_null()) {
			for (int i = 0; i < j["GameObjectMessages"].size(); i++) {
				scene.GOMessages.push_back({ j["GameObjectMessages"][i]["Emitter"], j["GameObjectMessages"][i]["Listener"] });
			}
		}
	}
	i.close();
	LogSystem::shutdownSingleton();

	return &scene;
}

// it reads the default information of the received prefab
void JsonReader::ReadPrefab(string name, GOType& gameObject) {
	ifstream i(routePrefabs);

	if (i.is_open()) {

		json j;
		i >> j;

		if (!j[name].is_null()) {
			gameObject.GOName = name;
			int i = 0;
			// we read the default constructor parameters (dont needed)
			if (!j[name][0]["GOParameters"].is_null()) {
				vector<string>goParams = j[name][0]["GOParameters"];
				gameObject.GOParameters = goParams;
				i = 1;
			}
			// we read each component with its default parameters
			for (i; i < j[name].size(); i++) {
				if (!j[name][i]["Name"].is_null()) {
					gameObject.components.push_back({ j[name][i]["Name"], j[name][i]["Parameters"] });
				}
			}
			gameObject.compMessages = MessagesType();
		}
		else LogSystem::getSingleton()->Log("El prefab \"" + name + "\" no existe");
	}
}

// it adds to the scene the GameObjects that will form the ground
void JsonReader::ReadMap(string level) {
	ifstream i(routeLevel + level + ".txt");

	if (i.is_open()) {
		int r, c;
		i >> r >> c;
		for (int k = 0; k < r; k++) {
			string floor; i >> floor;
			for (int j = 0; j < c; j++) {
				GOType go;
				// two types of gamobject: normal floor and falling floor
				// each of them read from its prefab
				if (floor[j]-48 == 0) {
					ReadPrefab("Floor", go);
					scene.gameObjects.push_back(go);
				}
				else if (floor[j]-48 == 1) {
					ReadPrefab("FallingFloor", go);
					scene.gameObjects.push_back(go);
				}
			}
		}
	}

	i.close();
}

// it returns the iterator of the component in the components list
CompType::iterator JsonReader::findComponent(CompType& components, string name)
{
	CompType::iterator it = components.begin();
	while (it != components.end() && it->compName != name ) {
		it++;
	}
	if(it == components.end())LogSystem::getSingleton()->Log("El componente \"" + name + "\" no existe");

	return it;
}

