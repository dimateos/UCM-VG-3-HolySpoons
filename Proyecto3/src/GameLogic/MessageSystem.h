//Nap_Time_Studios
#ifndef MESSAGE_SYSTEM_H_
#define MESSAGE_SYSTEM_H_

#include <list>
#include "GameObject.h"
#include "Component.h"

using namespace std;

class MessageSystem
{
private:
	static MessageSystem* instance_;
	list<GameObject*> targets_;

	MessageSystem();
	~MessageSystem();

public:
	static MessageSystem* getSingleton(); // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();

	void updateTargets(list<GameObject*> targets);
	
	void sendMessageName(Message* msg, string name);
	void sendMessageType(Message* msg, string type);
	void sendMessageGroup(Message* msg, string group);

	void sendMessageComponentName(Message* msg, string name, string cname);
	void sendMessageComponentType(Message* msg, string name, string ctype);
	void sendMessageComponentGroup(Message* msg, string name, string cgroup);
	
};

#endif /* MESSAGE_SYSTEM_H_ */