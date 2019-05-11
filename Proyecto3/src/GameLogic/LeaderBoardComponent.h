#ifndef LEADERBOARDCOMPONENT_H_
#define LEADERBOARDCOMPONENT_H_

#include "Component.h"

class LeaderBoardComponent: public Component
{
private:
	std::string jsonName;
	int LIMIT = 5;
	std::vector<std::pair<string, int>> punt;
	//std::vector<double> times;
	void saveToJSON();
	void readFromJSON();
	void update(std::string name, int newPunt);
public:
	virtual ~LeaderBoardComponent() { saveToJSON(); };
	LeaderBoardComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};

	virtual void setUp();
	virtual void receive(Message* msg);
};

#endif //LEADERBOARDCOMPONENT_H_