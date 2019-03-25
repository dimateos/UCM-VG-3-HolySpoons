#ifndef COMPONENT_FACTORY_H_
#define COMPONENT_FACTORY_H_

#include <vector>
#include <string>

using namespace std;
class Component;

// it parses component names (string) to components
// and creates them with their parameters
class ComponentFactory
{
private:
	static ComponentFactory* instance_; //singleton pattern
	ComponentFactory() {}
	virtual ~ComponentFactory() {}

public:
	static ComponentFactory* getSingleton();   // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();
	Component* ParseComponent(string componentName, std::vector<string> params);
};

#endif /* COMPONENT_FACTORY_H_ */
