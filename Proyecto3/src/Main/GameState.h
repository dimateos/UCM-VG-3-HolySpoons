//Nap_Time_Studios

#pragma once

#include <GameObject.h>
#include <iostream> //debugging for now
#include <string>
#include <list>

class GameState
{
public:
	GameState();
	virtual ~GameState();
	virtual bool handleEvents(SDL_Event evt);
	virtual void update(float time);

	//void sendToGSM(Message* msg);

	inline string setStateID(string id) { stateID_ = id; };
	inline string getStateID() { stateID_; };

protected:
	virtual void initState() = 0;
	virtual void closeState() = 0;

	//void killDeadObjects();

	std::list<GameObject*> gameObjects_;

	string stateID_ = "undefinedID";
};