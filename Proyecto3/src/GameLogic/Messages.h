#ifndef MESSAGES_H_
#define MESSAGES_H_

enum MessageId 
{
	// AQUI TIPOS DE MENSAJES QUE QUERAMOS QUE HAYA
};

struct Message {
	Message(MessageId id) : id_(id) { }
	MessageId id_;
};

#endif /* MESSAGES_H_ */
