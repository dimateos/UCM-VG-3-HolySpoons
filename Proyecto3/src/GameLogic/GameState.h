//Nap_Time_Studios
#pragma once

#include "GameObject.h"
#include <string>
#include <list>

class GameState : public Initiable
{
public:
	GameState();
	GameState(std::list<GameObject*>);
	virtual ~GameState();

	virtual void setUp();
	void addGameObject(GameObject* o);
	inline GameObject* getPlayer() const { return player_; };
	void setPlayer(GameObject* player);

	virtual bool handleEvents(SDL_Event evt);
	virtual void update(double time);

	//void sendToGSM(Message* msg);
	inline void setStateID(string id) { stateID_ = id; };
	inline string getStateID() { stateID_; };
	std::list<GameObject*>* getGameObjects(){ return &gameObjects_; };

protected:
	virtual void setDown();
	//void killDeadObjects();

	GameObject* player_;

	std::list<GameObject*> gameObjects_;
	string stateID_ = "undefinedID";
};