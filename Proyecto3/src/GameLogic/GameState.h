//Nap_Time_Studios
#pragma once

#include <string>
#include <list>
using namespace std;

class GameObject; //fowarding
typedef union SDL_Event;

#include "Initiable.h"
class GameState : public Initiable
{
public:
	inline GameState() {};
	virtual ~GameState();
	virtual void setUp();

	virtual bool handleEvents(SDL_Event evt);
	virtual void update(double time);

	//void sendToGSM(Message* msg);
	inline void setStateID(string id) { stateID_ = id; };
	inline string getStateID() { return stateID_; };

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
	inline GameObject* getGM() const { return gm_; };
	inline void setPlayer(GameObject* player) { player_ = player; addGameObject(player_); };
	inline void setGM(GameObject* gm) { gm_ = gm; addGameObject(gm_); };

protected:
	virtual void setDown();
	void killDeadObjects();

	GameObject* player_;
	GameObject* gm_;

	std::list<GameObject*> gameObjects_;
	string stateID_ = "undefinedID";
};