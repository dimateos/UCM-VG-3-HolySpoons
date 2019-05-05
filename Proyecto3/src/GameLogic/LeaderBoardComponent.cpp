#include "LeaderBoardComponent.h"
#include <fstream>
#include <iomanip>
#include <LogSystem.h>
#include "Messages.h"

using json = nlohmann::json;

void LeaderBoardComponent::saveToJSON()
{
	std::ofstream i(jsonName); //archivo donde se va a guardar
	nap_json j;

	//save things
	for (int i = 0; i < punt.size(); i++) {
		j["Nombres"][i] = punt[i].first;
		j["Puntuaciones"][i] = punt[i].second;
	}

	i << std::setw(3) << j; //pretty identacion para leer mejor el archivo
	i.close(); //cierra el flujo
}

//operador comparador
struct Pair_Comparator
{
	bool operator()(const std::pair<string, int> p1, const std::pair<string, int> p2) const
	{
		return p1.second > p2.second;
	}
};

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
			punt.push_back({ j["Nombres"][i], j["Puntuaciones"][i] });
		}
	}
}

void LeaderBoardComponent::update(std::string name, int newPunt)
{
	if (punt.size() < LIMIT) {
		punt.push_back({ name, newPunt });
		std::sort(punt.begin(), punt.end(), Pair_Comparator());
	}
	else {
		auto it = punt.begin();
		bool found = false;

		while (it != punt.end() && !found) {
			if (newPunt > (*it).second) { //vector is sorted
				//insert
				it = punt.insert(it, { name, newPunt });
				punt.pop_back(); //pops the last element

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
	if (msg->id_ == PLAYER_DEAD) {
		Msg_PLAYER_DEAD* p_msg = static_cast<Msg_PLAYER_DEAD*>(msg);
		update(p_msg->name_, p_msg->score_);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(LeaderBoardComponent);
