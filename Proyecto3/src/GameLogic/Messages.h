#ifndef MESSAGES_H_
#define MESSAGES_H_

enum MessageId 
{
	// DIFFERENT MESSAGE IDs
};

struct Message {
	Message(MessageId id) : id_(id) { }
	MessageId id_;
};

#endif /* MESSAGES_H_ */
