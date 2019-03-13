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
	GameState(std::list<GameObject*>);
	virtual ~GameState();

	virtual bool handleEvents(SDL_Event evt);
	virtual void update(float time);

	inline void addGameObject(GameObject* o) {
		gameObjects_.push_back(o);
	}

	//void sendToGSM(Message* msg);

	inline string setStateID(string id) { stateID_ = id; };
	inline string getStateID() { stateID_; };

protected:
	//maybe pure virtual and a few child classes
	virtual void initState();
	virtual void closeState();

	//void killDeadObjects();

	std::list<GameObject*> gameObjects_;

	string stateID_ = "undefinedID";
};