#ifndef JSON_READER_H_
#define JSON_READER_H_

#include <string>
#include <vector>
using namespace std;

//-----------------------------READING JSON INFO-----------------------------------

// CompType = vector of the name of each component
// and another vector with the paramaters of that component
struct CompStruct {
	string compName;
	std::vector<string> compParameters;
};
using CompType = std::vector<CompStruct>;

// MessagesType = vector with the name of the Emitter
// and the name of the Listener that will exchange messages
struct MessagesStruct {
	string emitterName, listenerName;
};
using MessagesType = std::vector<MessagesStruct>;

// GOType = cointains the name of the GameObject and
// three vectors: one with its constructor parameters,
// one with the components of the GameObject and another one
// with the components that will be Emitters and Listeners
struct GOStruct {
	string GOName;
	std::vector<string> GOParameters;
	CompType components;
	MessagesType compMessages;
};
using GOType = GOStruct;

// Scene_type = cointains a vector with the GameObjects of the scene
// and the GameObjects that will be Emitters and Listeners
struct SceneStruct {
	std::vector<GOType> gameObjects;
	MessagesType GOMessages;
};
using Scene_Type = SceneStruct;

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

	Scene_Type scene; // all the information of the scene (GameObjects, etc.)

	// private methods
	JsonReader();
	~JsonReader();

	// auxiliar methods
	void ReadPrefab(string name, GOType& go);
	void ReadMap(string level);
	void setTilePosition(int r, int c, int i, int j, GOType& go);
	CompType::iterator findComponent(CompType& components, string name);

public:
	static JsonReader* getSingleton();   // you can obtain/shutdown the instance of the singleton
	static void shutdownSingleton();
	Scene_Type* ReadLevel(string level); // reads the file called "level" and return the scene information
};

#endif /* JSON_READER_H_ */

// Here is an example of reading a level (.json) and creating gameobjects from it.
// This must be in GameState in the future (the place where we will read levels and create the stuff)
	/*
	JsonReader* jsReader = JsonReader::getSingleton();
	Scene_Type* scene = jsReader->ReadLevel("Example00");

	ComponentFactory* factory = ComponentFactory::getSingleton();

	std::vector<GameObject*> gameObjects;
	for (int i = 0; i < scene->gameObjects.size(); i++) {
		gameObjects.push_back(new GameObject()); //IMPORTANTE: cuando cada entidad tenga parametros en la constructora se pondran aqui usando scene.gameObjects[i].GOParams
		for (int j = 0; j < scene->gameObjects[i].components.size(); j++) {
			gameObjects[i]->addComponent(factory->ParseComponent(scene->gameObjects[i].components[j].compName, scene->gameObjects[i].components[j].compParameters));
		}
	}

	for (int i = 0; i < scene->gameObjects.size(); i++) {
		gameObjects[i]->update(0);
		delete gameObjects[i];
	}

	scene = nullptr;

	ComponentFactory::shutdownSingleton();
	JsonReader::shutdownSingleton();
	*/