#ifndef LISTENER_H_
#define LISTENER_H_

#include "Messages.h"

// heredar de Listener para recibir mensajes
class Listener {
public:
	Listener();
	virtual void receive(Message* msg) = 0;
	virtual ~Listener();
};

#endif /* LISTENER_H_ */
