#ifndef EMITTER_H_
#define EMITTER_H_

#include "Listener.h";
#include <vector>
#include "Messages.h"

// it allows to send messages
class Emitter {
private:
	// listeners list that will receive the message
	std::vector<Listener*> listeners_;

public:
	Emitter();

	virtual void send(Message* msg);
	virtual void registerObserver(Listener* o);

	virtual ~Emitter();
};

#endif /* EMITTER_H_ */
