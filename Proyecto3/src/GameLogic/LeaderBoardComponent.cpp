#include "LeaderBoardComponent.h"
#include <fstream>
#include <iomanip>
#include <LogSystem.h>

using json = nlohmann::json;

void LeaderBoardComponent::saveToJSON()
{
	std::ofstream i(jsonName); //archivo donde se va a guardar
	nap_json j;

	//save things
	for (int i = 0; i < punt.size(); i++) {
		j["Puntuaciones"][i] = punt[i];
	}
	for (int i = 0; i < names.size(); i++) {
		j["Nombres"][i] = names[i];
	}

	i << std::setw(3) << j; //pretty identacion para leer mejor el archivo
	i.close(); //cierra el flujo
}

void LeaderBoardComponent::readFromJSON()
{
	ifstream file(jsonName);
	if (!file.is_open()) {
		LogSystem::Log("El archivo " + jsonName + "no existe aun", LogSystem::JSON);
	}
	else {
		//parse it
		nap_json j;
		file >> j;
		for (int i = 0; i < j["Puntuaciones"].size(); i++) {
			punt.push_back(j["Puntuaciones"][i]);
		}
		for (int i = 0; i < j["Nombres"].size(); i++) {
			names.push_back(j["Nombres"][i]);
		}
	}
}

void LeaderBoardComponent::update(int newPunt)
{
	if (punt.size() < LIMIT) {
		punt.push_back(newPunt);
		std::sort(punt.begin(), punt.end(), greater<int>());
		//name???
		//better if we have one vector{ int, string }
	}
	else {
		auto it = punt.begin();
		bool found = false;

		while (it != punt.end() && !found) {
			if (newPunt > *it) { //vector is sorted
				//insert
				it = punt.insert(it, newPunt);
				punt.pop_back(); //pops the last element

				//update names??

				found = true;
			}
			else {
				it++;
			}
		}
	}
}

void LeaderBoardComponent::setUp() {
	if (isInited()) return;
	setInited();

	readFromJSON();
}

void LeaderBoardComponent::receive(Message * msg)
{
}

#include "GOFactory.h"
REGISTER_TYPE(LeaderBoardComponent);
