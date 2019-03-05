#ifndef JSON_READER_H_
#define JSON_READER_H_

#include <string>
#include <vector>

using namespace std;

class JsonReader
{
private:
	string routeLevel = "..\\exes\\Assets\\Levels\\";
	string routePrefabs = "..\\exes\\Assets\\Levels\\Prefabs.json";

	void ReadPrefab(string name, std::vector<pair<string, std::vector<string>>>& comps);

public:
	JsonReader() {}
	void ReadLevel(string level);
	~JsonReader() {}
};

#endif /* JSON_READER_H_ */
