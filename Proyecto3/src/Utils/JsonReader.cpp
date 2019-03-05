#include "JsonReader.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void JsonReader::ReadLevel(string level) {
	ifstream i(routeLevel + level + ".json");

	if (i.is_open()) { // Para que no intente abrir archivos que no existen

		json j;
		i >> j;

		if (!j["GameObjects"].is_null()) {
			for (int i = 0; i < j["GameObjects"].size(); i++) {
				if (!j["GameObjects"][i]["Name"].is_null()) {
					string prefabName = j["GameObjects"][i]["Name"];
					vector<pair<string, vector<string>>> components;
					ReadPrefab(prefabName, components);
				}
			}
		}
	}
}

void JsonReader::ReadPrefab(string name, vector<pair<string, vector<string>>>& comps) {
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