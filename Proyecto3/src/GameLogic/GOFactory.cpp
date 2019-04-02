#include "GOFactory.h"
#include "LogSystem.h"

GameObject * GOFactory::ParseGO(GOStruct * cfg) {
	auto go = new GameObject(cfg->go_cfg);

	//all its comps created cfg and added
	go->addComponent(ParseComponents(go, cfg->components_cfg));

	return go;
}

GameObject * GOFactory::GetGOPrefab(std::string const & name) {
	return ParseGO(JsonReader::getSingleton()->getPrefab(name));
}

// parses all the comps in nap_json * component_cfg
std::list<Component*> GOFactory::ParseComponents(GameObject * o, nap_json & components_cfg) {
	std::list<Component*> comps;

	//iterate over the named keys which are the comps names
	for (auto it = components_cfg.begin(); it != components_cfg.end(); it++) {
		//set the key as name and part of the id and parse the component
		(*it)["id"]["name"] = it.key();
		comps.push_back(ParseComponent(o, (*it)));
	}

	return comps;
}

//all Components
#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "TestComponent.h"
#include "OverlayComponent.h"
#include "FloatingEnemyComponent.h"
#include "FPSCamera.h"
#include "AutoRotationComponent.h"

// it receives the nap_json * component_cfg with its name and cfg.
// When you add a new component to the factory, you parse the unique type within cfg["id"]["type"]
//		and create the new component. You hace acces to the gameObject in case you need its transform etc
Component * GOFactory::ParseComponent(GameObject * o, nap_json const & component_cfg) {
	std::string type = component_cfg["id"]["type"];

	//now the switch
	if (type == "Phys") return new PhysicsComponent(component_cfg, o->getTransPtr()); //example of one comp using the go reference
	else if (type == "Render") return new RenderComponent(component_cfg);
	else if (type == "Test") return new TestComponent(component_cfg);
	else if (type == "Overlay") return new OverlayComponent(component_cfg);
	else if (type == "FPSCam") return new FPSCamera(component_cfg);
	else if (type == "FloatingEnemy") return new FloatingEnemyComponent(o, component_cfg);
	else if (type == "AutoRotation") return new AutoRotationComponent(component_cfg);

	else LogSystem::Log("Undefined component type " + type + " ignoring it...", LogSystem::JSON);
	return nullptr;
}
