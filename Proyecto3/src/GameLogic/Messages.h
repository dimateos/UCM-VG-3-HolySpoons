//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;
class GameObject;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//STATES
	PUSH_STATE,
	POP_STATE,
	QUIT_GAME,

	//UI
	ACTIVATE_POST_PROCESSING_HEALTH,
	DEACTIVATE_POST_PROCESSING_HEALTH,
	ACTIVATE_UI,
	CHECK_HP,
	END_INV,
	HP_DAMAGE,
	ENEMY_DAMAGE,
	PLAYER_DEAD,

	//PHYSX
	PX_USERPTR,
	TRIGGER_ENTER,
	TRIGGER_EXIT,

	//ENEMY GAMELOGIC
	RESET_PULL,
	ADD_SPAWNER,
	RESET_SPAWNER,
	ACTIVE_SPAWNER,
	ADD_ENEMY,

	//GAMELOGIC
	DEACTIVATE_OBJECT,
	BULLET_HIT,
	RESET_HP,
	UPGRADE_TAKEN,
	ADD_HP,
	ADD_SPEED,
	RESET_JUMP,
	SPRINT_ON,
	SPRINT_OFF,


	//CHANGE STATES
	STATE_IN,
	STATE_OUT,

	//SOUNDS
	PLAY_SOUND,
	STOP_SOUND,



};

struct Message {
	inline Message(MessageId id) : id_(id) { }
	MessageId id_;
};

struct Msg_PX_userPtr: public Message
{
	inline Msg_PX_userPtr(nap_transform *trans) : Message(PX_USERPTR), trans_(trans) {}
	nap_transform *trans_;
};

struct Msg_BULLET_HIT :public Message {
	inline Msg_BULLET_HIT(int score, bool enemy) : Message(BULLET_HIT), score_(score), enemy_(enemy) { }
	int score_;
	bool enemy_;
};

struct Msg_HP_DAMAGE :public Message {
	inline Msg_HP_DAMAGE(int damage) : Message(HP_DAMAGE), damage_(damage) { }
	int damage_;
};

struct Msg_UPGRADE_TAKEN :public Message {
	inline Msg_UPGRADE_TAKEN(int upgrade) : Message(UPGRADE_TAKEN), upgrade_(upgrade) { }
	int upgrade_;
};

struct Msg_PLAYER_DEAD :public Message {
	inline Msg_PLAYER_DEAD(std::string name, int score) : Message(PLAYER_DEAD), name_(name), score_(score) { }
	std::string name_;
	int score_;
};

struct Msg_RESET_SPAWNER :public Message {
	inline Msg_RESET_SPAWNER(int round) : Message(RESET_SPAWNER), round_(round) { }
	int round_;
};

struct Msg_ADD_ENEMY :public Message {
	inline Msg_ADD_ENEMY(int num) : Message(ADD_ENEMY), num_(num) { }
	int num_;
};

struct Msg_ADD_SPAWNER :public Message {
	inline Msg_ADD_SPAWNER(GameObject* spawner) : Message(ADD_SPAWNER), spawner_(spawner) { }
	GameObject* spawner_;
};

struct Msg_Trigger_Enter :public Message {
	inline Msg_Trigger_Enter(GameObject* msg_emitter) : Message(TRIGGER_ENTER), msg_emitter_(msg_emitter) { }
	GameObject* msg_emitter_;
};

struct Msg_Trigger_Exit :public Message {
	inline Msg_Trigger_Exit(GameObject* msg_emitter) : Message(TRIGGER_EXIT), msg_emitter_(msg_emitter) { }
	GameObject* msg_emitter_;
};

#endif /* MESSAGES_H_ */
