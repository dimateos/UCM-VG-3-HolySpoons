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

	virtual bool handleEvents(SDL_Event evt);
	virtual void update(double time);

	//void sendToGSM(Message* msg);
	inline void setStateID(string id) { stateID_ = id; };
	inline string getStateID() { stateID_; };

	//gameObjects methods
	void addGameObject(GameObject* go);
	void addGameObject(std::list<GameObject*> gos);
	void delGameObject(std::string name);
	void delGameObject(GameObject* go);
	void delGameObject(std::list<GameObject*> gos);

	GameObject* getGameObject(std::string name);
	inline std::list<GameObject*>* getGameObjects(){ return &gameObjects_; };
	inline void clearGameObject() { gameObjects_.clear(); };

	//specific for player
	inline GameObject* getPlayer() const { return player_; };
	inline void setPlayer(GameObject* player) { player_ = player; addGameObject(player_); };

protected:
	virtual void setDown();
	//void killDeadObjects();

	GameObject* player_;

	std::list<GameObject*> gameObjects_;
	string stateID_ = "undefinedID";
};