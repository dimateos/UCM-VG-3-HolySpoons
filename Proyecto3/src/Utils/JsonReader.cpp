#include "JsonReader.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Scene_Type* JsonReader::ReadLevel(string level) {
	ifstream i(routeLevel + level + ".json");

	Scene_Type scene = Scene_Type();

	if (i.is_open()) { // Para que no intente abrir archivos que no existen

		json j;
		i >> j;

		MessagesType goMessages;

		if (!j["GameObjects"].is_null()) {
			for (int i = 0; i < j["GameObjects"].size(); i++) {
				CompType components;
				MessagesType componentMessages;

				if (!j["GameObjects"][i]["Name"].is_null()) {
					string prefabName = j["GameObjects"][i]["Name"];
					ReadPrefab(prefabName, components);
				}

				if (!j["GameObjects"][i]["Custom"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["Custom"].size(); x++) {
						CompType::iterator it;
						it = findComponent(components, j["GameObjects"][i]["Custom"][x]["Name"]);

						try {
							for (int k = 0; k < j["GameObjects"][i]["Custom"][x]["Parameters"].size(); k++) {
								if (j["GameObjects"][i]["Custom"][x]["Parameters"][k] != "-") {
									string parameter = j["GameObjects"][i]["Custom"][x]["Parameters"][k];
									it->second[k] = parameter;
								}
							}
						}
						catch (exception ex){
							printf(ex.what());
						}
					}
				}

				if (!j["GameObjects"][i]["ComponentMessages"].is_null()) {
					for (int x = 0; x < j["GameObjects"][i]["ComponentMessages"].size(); x++) {
						componentMessages.push_back({ j["GameObjects"][i]["ComponentMessages"][x]["Emitter"], j["GameObjects"][i]["ComponentMessages"][x]["Listener"] });
					}
				}

				/*cout << j["GameObjects"][i]["Name"] << endl;
				for (int h = 0; h < components.size(); h++) {
					cout << components[h].first << " ";
					for (int z = 0; z < components[h].second.size(); z++) {
						cout << components[h].second[z] << " ";
					}
					cout << endl;
				}

				for (int h = 0; h < componentMessages.size(); h++) {
					cout << "Emitter: " << componentMessages[h].first << " Listener: " << componentMessages[h].second << endl;
				}
				cout << endl;*/
			}
		}
		if (!j["GameObjectMessages"].is_null()) {
			for (int i = 0; i < j["GameObjectMessages"].size(); i++) {
				goMessages.push_back({ j["GameObjectMessages"][i]["Emitter"], j["GameObjectMessages"][i]["Listener"] });
				//cout << "Emitter: " << goMessages[i].first << " Listener: " << goMessages[i].second << endl;
			}
		}
	}
	return &scene;
}

void JsonReader::ReadPrefab(string name, CompType& comps) {
	ifstream i(routePrefabs);

	if (i.is_open()) { // Para que no intente abrir archivos que no existen

		json j;
		i >> j;

		if (!j[name].is_null()) {
			for (int i = 0; i < j[name].size(); i++) {
				if (!j[name][i]["Name"].is_null()) {
					comps.push_back({ j[name][i]["Name"], j[name][i]["Parameters"] });
				}
			}
		}
	}
}

CompType::iterator JsonReader::findComponent(CompType& components, string name)
{
	CompType::iterator it = components.begin();
	while (it != components.end() && it->first != name ) {
		it++;
	}

	return it;
}

