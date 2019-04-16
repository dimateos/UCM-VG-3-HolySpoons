#ifndef GO_FACTORY_H_
#define GO_FACTORY_H_

#include <list>
#include "Component.h"
#include "JsonReader.h"

#define REGISTER_TYPE(klass) \
    class klass##Factory : public GOFactory { \
    public: \
        klass##Factory() \
        { \
            Component::registerType(#klass, this); \
        } \
        virtual Component* create() { \
            return new klass(); \
        } \
    }; \
    static klass##Factory global_##klass##Factory;

//all pointers so no .h needed
class GOFactory
{
public:
	GOFactory() {}
	// parses a GOType into a GO
	static GameObject* ParseGO(GOStruct & cfg);

	// gets the GO parsed of a prefab
	static GameObject* GetGOPrefab(std::string const & name);

	// parses a nap_json components_cfg into a list of configured components*
	static std::list<Component*> ParseComponents(GameObject *o, nap_json & components_cfg);

	// parse individual comp
	static Component* ParseComponent(GameObject *o, nap_json const & component_cfg);

	virtual Component* create() = 0;
};

#endif /* GO_FACTORY_H_ */
