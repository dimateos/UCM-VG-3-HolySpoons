#include "ComponentFactory.h"
#include "LogSystem.h"

// parses all the comps in nap_json * component_cfg
std::list<Component*> ComponentFactory::ParseComponents(GameObject * o, nap_json * components_cfg) {
	std::list<Component*> comps;

	//iterate over the named keys which are the comps names
	for (auto it = components_cfg->begin(); it != components_cfg->end(); it++) {
		//set the key as name and part of the id and parse the component
		(*it)["id"]["name"] = it.key();
		nap_json * cfg = &(*it);
		comps.push_back(ParseComponent(o, cfg));
	}

	return comps;
}

//all Components
#include "PhysicsComponent.h"
#include "RenderComponent.h"

// it receives the nap_json * component_cfg with its name and cfg.
// When you add a new component to the factory, you parse the unique type within cfg["id"]["type"]
//		and create the new component. You hace acces to the gameObject in case you need its transform etc
Component * ComponentFactory::ParseComponent(GameObject * o, nap_json * component_cfg) {
	std::string type = (*component_cfg)["id"]["type"];

	//now the switch
	if (type == "Phys") return new PhysicsComponent(*component_cfg, o->getTransPtr()); //example of one comp using the go reference
	else if (type == "Render") return new RenderComponent(*component_cfg);

	else LogSystem::Log("Undefined component type " + type + " ignoring it...", LogSystem::JSON);
	return nullptr;
}
