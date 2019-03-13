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

// it returns a Scene_type with all the information read
// from "level"
Scene_Type* JsonReader::ReadLevel(string level) {
	ifstream i(routeLevel + level + ".json");
	Scene_Type* scene = new Scene_Type();

	if (i.is_open()) {
		json j;
		i >> j;

		// reading of the gameobjects
		if (!j["GameObjects"].is_null()) {
			for (int i = 0; i < j["GameObjects"].size(); i++) {
				CompType components;
				MessagesType componentMessages;

				if (!j["GameObjects"][i]["Name"].is_null()) {
					string prefabName = j["GameObjects"][i]["Name"];
					ReadPrefab(prefabName, components); // reading of the prefab
				}

				// reading of the custom parameters
				if (!j["GameObjects"][i]["Custom"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["Custom"].size(); x++) {
						CompType::iterator it; string customName = j["GameObjects"][i]["Custom"][x]["Name"];
						it = findComponent(components, customName);

						if (it < components.end()) {
							for (int k = 0; k < j["GameObjects"][i]["Custom"][x]["Parameters"].size(); k++) {
								if (j["GameObjects"][i]["Custom"][x]["Parameters"][k] != "-") {
									string parameter = j["GameObjects"][i]["Custom"][x]["Parameters"][k];
									it->second[k] = parameter;
								}
							}
						}
					}
				}

				// reading of the components that will be listeners and emitters
				if (!j["GameObjects"][i]["ComponentMessages"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["ComponentMessages"].size(); x++) {
						string emitter = j["GameObjects"][i]["ComponentMessages"][x]["Emitter"];
						string listener = j["GameObjects"][i]["ComponentMessages"][x]["Listener"];
						if (findComponent(components, emitter) < components.end() && findComponent(components, listener) < components.end())
							componentMessages.push_back({ emitter, listener });
					}
				}
				scene->first.push_back({ j["GameObjects"][i]["Name"], {components, componentMessages } });
			}
		}

		// reading of the gameobjects that will be listeners and emitters
		if (!j["GameObjectMessages"].is_null()) {
			for (int i = 0; i < j["GameObjectMessages"].size(); i++) {
				scene->second.push_back({ j["GameObjectMessages"][i]["Emitter"], j["GameObjectMessages"][i]["Listener"] });
			}
		}
	}
	i.close();

	return scene;
}

// it reads the default information of the received prefab
void JsonReader::ReadPrefab(string name, CompType& comps) {
	ifstream i(routePrefabs);

	if (i.is_open()) {

		json j;
		i >> j;

		if (!j[name].is_null()) {
			for (int i = 0; i < j[name].size(); i++) {
				if (!j[name][i]["Name"].is_null()) {
					comps.push_back({ j[name][i]["Name"], j[name][i]["Parameters"] });
				}
			}
		}
		else LogSystem::getSingleton()->Log("El prefab \"" + name + "\" no existe");
	}
}

CompType::iterator JsonReader::findComponent(CompType& components, string name)
{
	CompType::iterator it = components.begin();
	while (it != components.end() && it->first != name ) {
		it++;
	}
	if(it == components.end())LogSystem::getSingleton()->Log("El componente \"" + name + "\" no existe");

	return it;
}

