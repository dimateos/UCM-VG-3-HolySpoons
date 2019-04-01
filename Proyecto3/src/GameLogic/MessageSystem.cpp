//Nap_Time_Studios
#include "MessageSystem.h"

MessageSystem* MessageSystem::instance_ = nullptr;

MessageSystem::MessageSystem()
{
}


MessageSystem::~MessageSystem()
{
}

MessageSystem * MessageSystem::getSingleton()
{
	if (instance_ == nullptr) {
		instance_ = new MessageSystem();
	}

	return instance_;
}

void MessageSystem::shutdownSingleton()
{
	if (instance_ != nullptr) 
	{
		delete instance_;
		instance_ = nullptr;
	}
}

void MessageSystem::updateTargets(list<GameObject*> targets)
{
	targets_ = targets;
}

void MessageSystem::sendMessageName(Message * msg, string name)
{	
	for (GameObject* o : targets_) {
		if (o->id().name_ == name)
			for (Component* c : o->getComponents())
				c->receive(msg);
	}
}

void MessageSystem::sendMessageType(Message * msg, string type)
{
	for (GameObject* o : targets_) {
		if (o->id().type_ == type)
			for (Component* c : o->getComponents())
				c->receive(msg);
	}
}

void MessageSystem::sendMessageGroup(Message * msg, string group)
{
	for (GameObject* o : targets_) {
		if (o->id().group_ == group)
			for (Component* c : o->getComponents())
				c->receive(msg);
	}
}

void MessageSystem::sendMessageComponentName(Message * msg, string name, string cname)
{
	for (GameObject* o : targets_) {
		if (o->id().name_ == name)
			for (Component* c : o->getComponents()) {
				if(c->id().name_ == cname)
					c->receive(msg);
			}
	}
}

void MessageSystem::sendMessageComponentType(Message * msg, string name, string ctype)
{
	for (GameObject* o : targets_) {
		if (o->id().name_ == name)
			for (Component* c : o->getComponents()) {
				if (c->id().type_ == ctype)
					c->receive(msg);
			}
	}
}

void MessageSystem::sendMessageComponentGroup(Message * msg, string name, string cgroup)
{
	for (GameObject* o : targets_) {
		if (o->id().name_ == name)
			for (Component* c : o->getComponents()) {
				if (c->id().group_ == cgroup)
					c->receive(msg);
			}
	}
}


