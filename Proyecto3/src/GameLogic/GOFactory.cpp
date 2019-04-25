#include "GOFactory.h"
#include "Component.h"
#include "LogSystem.h"

std::map<string, GOFactory*>* GOFactory::factories = nullptr;

void GOFactory::registerType(const string& name, GOFactory* factory) {
	if (factories == nullptr) factories = new std::map<string, GOFactory*>();
	GOFactory::factories->insert({ name, factory });
	//LogSystem::Log("Componente " + name + " registrado", LogSystem::FACTORY);
}

////////////////////////////////////////////////////////////////////////////////

GameObject * GOFactory::ParseGO(GOStruct & cfg) {
	auto go = new GameObject(cfg.go_cfg);

	//all its comps created cfg and added
	go->addComponent(ParseComponents(go, cfg.components_cfg));

	return go;
}

GameObject * GOFactory::GetGOPrefab(std::string const & name) {
	GameObject * go = nullptr;

	bool successs = false;
	auto go_struct = JsonReader::getSingleton()->getPrefab(name, successs);

	if (!successs) LogSystem::Log("El prefab " + name + " no fue encontrado... abortando parseo del GO", LogSystem::FACTORY);
	else go = ParseGO(go_struct);

	return go;
}

// parses all the comps in nap_json * component_cfg
std::list<Component*> GOFactory::ParseComponents(GameObject * o, nap_json & components_cfg) {
	std::list<Component*> comps;

	//iterate over the named keys which are the comps names
	for (auto it = components_cfg.begin(); it != components_cfg.end(); it++) {
		//set the key as name and part of the id and parse the component
		(*it)["id"]["name"] = it.key();

		//find in the map
		string type = (*it)["id"]["type"];
		auto index = GOFactory::factories->find(type);

		if (index != GOFactory::factories->end()) {
			comps.push_back(index->second->create((*it), o));
		}
		else LogSystem::Log("El componente " + type + " no fue encontrado... type mal puesto en json? olvidaste la macro?", LogSystem::FACTORY);
	}

	return comps;
}