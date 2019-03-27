#ifndef COMPONENT_FACTORY_H_
#define COMPONENT_FACTORY_H_

#include <list>
#include "Component.h"

//all pointers so no .h needed
class ComponentFactory
{
public:
	// parses a nap_json components_cfg into a list of configured components*
	static std::list<Component*> ParseComponents(GameObject *o, nap_json * components_cfg);

	// parse individual comp
	static Component* ParseComponent(GameObject *o, nap_json * component_cfg);
};

#endif /* COMPONENT_FACTORY_H_ */
