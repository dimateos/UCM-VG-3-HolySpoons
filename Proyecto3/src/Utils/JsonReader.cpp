#include "JsonReader.h"
#include "LogSystem.h"

#include <fstream>

#include "json.hpp"
using nap_json = nlohmann::json;

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
// from "level.json" (GameObjects) and "level.txt" (map/tiles)
Scene_Type* JsonReader::ReadLevel(string level) {

	ReadMap(level); // it reads the map/tiles ("level.txt"; it will be an amount of GameObjects)

	// it will read de GameObjects of the level ("level.json")
	ifstream file(routeLevel + level + ".json");
	if (file.is_open()) {
		nap_json j;
		file >> j;

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
						// find / check if the prefab has that component
						string customName = j["GameObjects"][i]["Custom"][x]["Name"];
						CompType::iterator it = findComponent(go.components, customName);

						if (it < go.components.end()) {
							for (int k = 0; k < j["GameObjects"][i]["Custom"][x]["Parameters"].size(); k++) {
								if (j["GameObjects"][i]["Custom"][x]["Parameters"][k] != "-") {
									string parameter = j["GameObjects"][i]["Custom"][x]["Parameters"][k];
									it->compParameters[k] = parameter;
								}
							}
						}
						else {
							string name = j["GameObjects"][i]["Name"];
							LogSystem::Log("La entidad " + name + " no tiene el componente "
								+ customName +" que se intenta modificar", LogSystem::JSON);
						}
					}
				}

				// we read the components that will be listeners and emitters
				if (!j["GameObjects"][i]["ComponentMessages"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["ComponentMessages"].size(); x++) {
						string emitter = j["GameObjects"][i]["ComponentMessages"][x]["Emitter"];
						string listener = j["GameObjects"][i]["ComponentMessages"][x]["Listener"];

						// find / check if the prefab has those components
						if (findComponent(go.components, emitter) < go.components.end()
							&& findComponent(go.components, listener) < go.components.end()) {
							go.compMessages.push_back({ emitter, listener });
						}
						else {
							LogSystem::Log("El componente emmiter " + emitter + " o listener " + listener + " no existe", LogSystem::JSON);
						}
					}
				}
				scene.gameObjects.push_back(go); // we push the new gameobject
			}
		}

		// reading of the gameobjects that will be listeners and emitters
		if (!j["GameObjectMessages"].is_null()) {
			for (int i = 0; i < j["GameObjectMessages"].size(); i++) {
				// here no check if they exist?
				scene.GOMessages.push_back({ j["GameObjectMessages"][i]["Emitter"], j["GameObjectMessages"][i]["Listener"] });
			}
		}
	}
	file.close();
	//LogSystem::shutdownSingleton(); //no¿

	return &scene;
}

// it reads the default information of the received prefab
void JsonReader::ReadPrefab(string name, GOType& go) {
	ifstream i(routePrefabs);

	if (i.is_open()) {

		nap_json j;
		i >> j;

		if (!j[name].is_null()) {
			go.GOName = name;
			int i = 0;
			// we read the default constructor parameters (dont needed)
			if (!j[name][0]["GOParameters"].is_null()) {
				vector<string>goParams = j[name][0]["GOParameters"];
				go.GOParameters = goParams;
				i = 1;
			}
			// we read each component with its default parameters
			for (i; i < j[name].size(); i++) {
				if (!j[name][i]["Name"].is_null()) {
					go.components.push_back({ j[name][i]["Name"], j[name][i]["Parameters"] });
				}
			}
			go.compMessages = MessagesType();
		}
		else LogSystem::Log("El prefab \"" + name + "\" no existe", LogSystem::JSON);
	}
}

// it adds to the scene the GameObjects (tiles) that will form the floor
void JsonReader::ReadMap(string level) {
	ifstream in(routeLevel + level + ".txt");

	if (in.is_open()) {
		int r, c;
		in >> r >> c;
		for (int i = 0; i < r; i++) {
			string floor; in >> floor;
			for (int j = 0; j < c; j++) {
				GOType go;
				// two types of gamobject: normal floor and falling floor
				// each of them read from its prefab
				if (floor[j]-48 == 0) {
					ReadPrefab("Tile", go);
					setTilePosition(r, c, i, j, go);
					scene.gameObjects.push_back(go);
				}
				else if (floor[j]-48 == 1) {
					ReadPrefab("FallingTile", go);
					setTilePosition(r, c, i, j, go);
					scene.gameObjects.push_back(go);
				}
			}
		}
	}

	in.close();
}

// set the physical position of the tile parsing its logic position (i, j)
void JsonReader::setTilePosition(int r, int c, int i, int j, GOType& go) {
	const float POSITION_FACTOR_C = 1.1f;
	const float POSITION_FACTOR_R = 1.1f;

	float x, y, z;
	x = -((c / 2.0f) * POSITION_FACTOR_C - (POSITION_FACTOR_C / 2.0f)) + j * POSITION_FACTOR_C;
	y = 0;
	z = (r / 2.0f) * POSITION_FACTOR_R - (POSITION_FACTOR_R / 2.0f) - i * POSITION_FACTOR_R;
	try {
		if (go.GOParameters.size() >= 3) {
			go.GOParameters[0] = to_string(x); go.GOParameters[1] = to_string(y); go.GOParameters[2] = to_string(z);
		}
		else throw "Tienes que poner al menos tres parametros en GOParameters de cada prefab Tile (Prefabs.json) para su posicion";
	}
	catch (const char* e) {
		LogSystem::Log(e, LogSystem::JSON);
	}
}

// it returns the iterator of the component in the components list
CompType::iterator JsonReader::findComponent(CompType& components, string name)
{
	CompType::iterator it = components.begin();
	while (it != components.end() && it->compName != name ) {
		it++;
	}
	if(it == components.end()) LogSystem::Log("El componente \"" + name + "\" no existe", LogSystem::JSON);

	return it;
}

