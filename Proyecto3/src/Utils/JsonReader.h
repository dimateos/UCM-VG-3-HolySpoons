#ifndef JSON_READER_H_
#define JSON_READER_H_

#include <string>
#include <vector>

using namespace std;

//-----------------------------READING JSON INFO-----------------------------------

// CompType = vector of pairs with the name of each component
// and another vector with the paramaters of that component
using CompType = std::vector<pair<string, std::vector<string>>>;

// MessagesType = vector of pairs with the name of the Emitter
// and the name of the Listener that will exchange messages
using MessagesType = std::vector<pair<string, string>>;

// GameObjectType = pair that cointains the name of the GameObject and
// two vectors, one with the components of the GameObject and another one
// with the components that will be Emitters and Listeners
using GameObjectType = pair<string, pair<CompType, MessagesType>>;

// Scene_type = pair that cointains a vector with the GameObjects of the scene
// and the GameObjects that will be Emitters and Listeners 
using Scene_Type = pair<std::vector<GameObjectType>, MessagesType>;

//---------------------------------------------------------------------------------

// this class reads json files (singleton)
class JsonReader
{
private:
	static JsonReader* instance_; //singleton pattern

	// routes to several files that will be read
	// every new level file must fit in "routeLevel"
	string routeLevel = "..\\exes\\Assets\\Levels\\";
	string routePrefabs = "..\\exes\\Assets\\Levels\\Prefabs.json";

	// private methods
	JsonReader() {}
	~JsonReader() {}
	void ReadPrefab(string name, CompType& comps);
	CompType::iterator findComponent(CompType& components, string name);

public:
	static JsonReader* getSingleton();   // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();
	Scene_Type* ReadLevel(string level); // reads the file called "level"
};

#endif /* JSON_READER_H_ */

// Here is an example of reading a level (.json) and creating gameobjects from it.
// This must be in GameState in the future (the place where we will read levels and create the stuff)
	/*
	JsonReader* jsReader = JsonReader::getSingleton();
	Scene_Type* scene = jsReader->ReadLevel("Example00");

	ComponentFactory* factory = ComponentFactory::getSingleton();

	std::vector<GameObject*> gameObjects;
	for (int i = 0; i < scene->first.size(); i++) {
		gameObjects.push_back(new GameObject());
		for (int j = 0; j < scene->first[i].second.first.size(); j++) {
			gameObjects[i]->addComponent(factory->ParseComponent(scene->first[i].second.first[j].first, scene->first[i].second.first[j].second));
		}
	}

	for (int i = 0; i < scene->first.size(); i++) {
		gameObjects[i]->update(0);
		delete gameObjects[i];
	}
	delete scene;
	ComponentFactory::shutdownSingleton();
	JsonReader::shutdownSingleton();
	*/