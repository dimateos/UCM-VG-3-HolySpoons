#include "Emitter.h"

Emitter::Emitter() : listeners_() { }
Emitter::Emitter(std::list<Listener*> listeners) : listeners_(listeners) {}

Emitter::~Emitter() { }

void Emitter::send(Message* msg) {
	for( auto lis : listeners_ ) {
		lis->receive(msg);
	}
}

void Emitter::registerObserver(Listener* lis) {
	listeners_.push_back(lis);
}
void Emitter::registerObserver(std::list<Listener*> lis) {
	for (auto l : lis) listeners_.push_back(l);
}

void Emitter::deregisterObserver(Listener * lis) {
	auto it = std::find(listeners_.begin(), listeners_.end(), lis);
	if (it != listeners_.end()) listeners_.erase(it);
}
void Emitter::deregisterObserver(std::list<Listener*> lis) {
	for (auto l : lis) {
		auto it = std::find(listeners_.begin(), listeners_.end(), l);
		if (it != listeners_.end()) listeners_.erase(it);
	}
}
