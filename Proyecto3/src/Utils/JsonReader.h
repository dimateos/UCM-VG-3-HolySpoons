#ifndef JSON_READER_H_
#define JSON_READER_H_

#include <string>
#include <vector>

using namespace std;
using CompType = std::vector<pair<string, std::vector<string>>>;
using MessagesType = std::vector<pair<string, string>>;
using Scene_Type = pair<std::vector<pair<CompType, MessagesType>>, MessagesType>;

class JsonReader
{
private:
	string routeLevel = "..\\exes\\Assets\\Levels\\";
	string routePrefabs = "..\\exes\\Assets\\Levels\\Prefabs.json";

	void ReadPrefab(string name, CompType& comps);
	CompType::iterator findComponent(CompType& components, string name);

public:
	JsonReader() {}
	Scene_Type* ReadLevel(string level);
	~JsonReader() {}
};

#endif /* JSON_READER_H_ */
