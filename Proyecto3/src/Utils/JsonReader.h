#ifndef JSON_READER_H_
#define JSON_READER_H_

#include <string>
#include <list>
#include <map>
using namespace std;

#include "json.hpp"
using nap_json = nlohmann::json;

//-----------------------------READING JSON INFO-----------------------------------

// GOType = list of Gameobjects configurations (name, pos, etc) and comps cfgs
struct GOStruct {
	nap_json go_cfg;
	nap_json components_cfg;
};
using GOType = std::list<GOStruct>;

// SceneType = scene's name and GOType
struct SceneStruct {
	string SceneName;
	GOType gameObjects;
};

//---------------------------------------------------------------------------------

// this class reads json files (singleton)
class JsonReader
{
public:
	static JsonReader* getSingleton();   // you can obtain/shutdown the instance of the singleton
	static void shutdownSingleton();

	SceneStruct ReadLevel(string level); // reads the file called "level" and return the scene information


private:
	static JsonReader* instance_; //singleton pattern
	JsonReader();
	~JsonReader();

	// levels files must be in "routeLevel" and prefabs in "routePrefabs"
	string routeLevel = ".\\Assets\\Levels\\";
	string routePrefabs = ".\\Assets\\Levels\\Prefabs\\";

	// prefab preloading
	void preloadPrefabs();
	std::map<string, GOStruct*> prefabs;

	// auxiliar methods
	GOStruct* readGO(nap_json const & cfg);

	void ReadMap(string level);
	void setTilePosition(int r, int c, int i, int j, GOType& go);
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