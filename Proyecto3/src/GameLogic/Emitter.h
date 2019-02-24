#ifndef EMITTER_H_
#define EMITTER_H_

#include "Listener.h";
#include <vector>
#include "Messages.h"

//heredar de Emitter para enviar mensajes
class Emitter {
private:
	// lista de oyentes que pueden recibir mensajes de el
	std::vector<Listener*> listeners_;

public:
	Emitter();

	virtual void send(Message* msg);
	virtual void registerObserver(Listener* o);

	virtual ~Emitter();
};

#endif /* EMITTER_H_ */
