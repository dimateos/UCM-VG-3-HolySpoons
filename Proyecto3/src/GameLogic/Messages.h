//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//PHYSX
	PX_USERPTR,

	//UI
	ACTIVATE_UI,
	ADD_SCORE,
	HP_DAMAGE,
	BUTTON_START_GAME,

	//ENEMY GAMELOGIC
	HP_RESET
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

struct Msg_ADD_SCORE:public Message {
	inline Msg_ADD_SCORE(int score) : Message(ADD_SCORE), score_(score) { }
	int score_;
};

struct Msg_HP_DAMAGE :public Message {
	inline Msg_HP_DAMAGE(int damage) : Message(HP_DAMAGE), damage_(damage) { }
	int damage_;
};

#endif /* MESSAGES_H_ */
