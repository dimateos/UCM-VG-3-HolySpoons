//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//STATES
	PUSH_STATE,
	POP_STATE,
	QUIT_GAME,

	//UI
	ACTIVATE_UI,
	CHECK_HP,
	END_INV,
	HP_DAMAGE,
	ENEMY_DAMAGE,
	PLAYER_DEAD,

	//PHYSX
	PX_USERPTR,

	//ENEMY GAMELOGIC
	HP_RESET,
	RESET_PULL,
	RESET_HP,
	RESET_SPAWNER,
	ACTIVE_SPAWNER,
	ADD_ENEMY,
	BULLET_HIT,

	//CHANGE STATES
	STATE_CHANGED,

	//SOUNDS
	PLAY_SOUND,
	STOP_SOUND
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

struct Msg_PLAYER_DEAD :public Message {
	inline Msg_PLAYER_DEAD(std::string name, int score) : Message(PLAYER_DEAD), name_(name), score_(score) { }
	std::string name_;
	int score_;
};

struct Msg_RESET_SPAWNER :public Message {
	inline Msg_RESET_SPAWNER(int num) : Message(RESET_SPAWNER), num_(num) { }
	int num_;
};

struct Msg_ADD_ENEMY :public Message {
	inline Msg_ADD_ENEMY(int num) : Message(ADD_ENEMY), num_(num) { }
	int num_;
};

#endif /* MESSAGES_H_ */
