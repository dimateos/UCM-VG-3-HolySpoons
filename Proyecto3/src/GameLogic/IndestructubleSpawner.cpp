#include "IndestructubleSpawner.h"
#include "Pool.h"
#include "MessageSystem.h"
#include "GameStateMachine.h"

void IndestructibleSpawner::setUp() {
	if (FIND(cfg_, "spawnFactor")) spawnFactor = cfg_["spawnFactor"];
	else spawnFactor = 1;
	if (FIND(cfg_, "timeFactor")) timeFactor = cfg_["timeFactor"];
	else timeFactor = 10;

	if (FIND(cfg_, "spawnAlgorithm")) spawnAlgorithm = cfg_["spawnAlgorithm"];
	else 		spawnAlgorithm = "MUL";
	if (FIND(cfg_, "timeAlgorithm")) timeAlgorithm = cfg_["timeAlgorithm"];
	else 		timeAlgorithm = "MUL";

	Spawner::setUp();

	timeToSpawn = FINDnRETURN(cfg_, "timer", float, 10);
}

// updates the spawner depending on the round
void IndestructibleSpawner::upToRound()
{
	upNumToSpawn();  // updates the spawning algorithm
	upTimeToSpawn(); // updates the timer algorithm

	// update the number of enemies to kill of the GameManager
	MessageSystem::getSingleton()->sendMessageGameObject(&Msg_ADD_ENEMY(numToSpawn),
		GameStateMachine::getSingleton()->currentState()->getGM());
}

// spawning algorithms (you will choose one of them in the .json)
void IndestructibleSpawner::upNumToSpawn()
{
	if (spawnAlgorithm == "JUMPS") {
		numToSpawn = prevNumToSpawn + spawnFactor;
		prevNumToSpawn = numToSpawn;

	}
	else if (spawnAlgorithm == "LINEAR") {
		numToSpawn = round_ * spawnFactor;
	}
	else if (spawnAlgorithm == "LOG") {
		numToSpawn = (log(round_ + 3) / log(spawnFactor)) - 32;
	}
	else if (spawnAlgorithm == "EXP") {
		numToSpawn = pow(round_, spawnFactor);
	}
	else if (spawnAlgorithm == "MOD") {
		numToSpawn = prevNumToSpawn + 1;
		if (round_ % (int)spawnFactor != 0) numToSpawn = 0;
		else prevNumToSpawn = numToSpawn;
	}
}

// timer algorithms (you will choose one of them in the .json)
void IndestructibleSpawner::upTimeToSpawn()
{
	if (timeAlgorithm == "JUMPS") {
		timeToSpawn -= timeFactor;
		if (timeToSpawn < 1) timeToSpawn = 0.5;
	}
	else if (timeAlgorithm == "LINEAR") {
		timeToSpawn -= timeFactor / round_;
	}
	else if (timeAlgorithm == "LOG") {
		timeToSpawn -= (log(round_ + 1) / log(timeFactor));
	}
	else if (timeAlgorithm == "EXP") {
		timeToSpawn -= pow(round_, timeFactor);
	}

	t.setDuration(timeToSpawn);
}

void IndestructibleSpawner::update(GameObject * o, double time) {
	if (t.update(time)) {
		t.start();	//Timer reset

		// numToSpawn > 0 -> limit of enemies to spawn
		if (numToSpawn > 0) {
			spawn();
			numToSpawn--;
		}
	}
}

void IndestructibleSpawner::receive(Message * msg) {
	Spawner::receive(msg);

	// when you reset this spawner, you update its round to the new round
	if (msg->id_ == RESET_SPAWNER) {
		round_ = static_cast<Msg_RESET_SPAWNER*>(msg)->round_;
		upToRound(); // update algorithms
		t.start();
	}
}

#include "GOFactory.h"
REGISTER_TYPE(IndestructibleSpawner);
