//Nap_Time_Studios
#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//PHYSX
	PX_USERPTR,
	ACTIVATE_UI,
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

#endif /* MESSAGES_H_ */

#ifndef MESSAGES_H_
#define MESSAGES_H_

class nap_transform;

enum MessageId // DIFFERENT MESSAGE IDs
{
	//PHYSX
	PX_USERPTR,
	ACTIVATE_UI
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

#endif /* MESSAGES_H_ */
