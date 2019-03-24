//Nap_Time_Studios
#ifndef EMITTER_H_
#define EMITTER_H_

#include <list>
#include "Listener.h";

// it allows to send messages
class Emitter {
public:
	Emitter();
	Emitter(std::list<Listener*> listeners);
	virtual ~Emitter();

	virtual void send(Message* msg);
	virtual void registerObserver(Listener* lis);
	virtual void registerObserver(std::list<Listener*> lis);
	virtual void deregisterObserver(Listener* lis);
	virtual void deregisterObserver(std::list<Listener*> lis);

private:
	// listeners list that will receive the message
	std::list<Listener*> listeners_;
};

#endif /* EMITTER_H_ */
