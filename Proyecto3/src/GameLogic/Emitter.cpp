#include "Emitter.h"

Emitter::Emitter() : listeners_() { }

void Emitter::send(Message* msg) {
	for( auto lis : listeners_ ) {
		lis->receive(msg);
	}
}

void Emitter::registerObserver(Listener* o) {
	listeners_.push_back(o);
}

Emitter::~Emitter() { }
