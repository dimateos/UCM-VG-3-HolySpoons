//Nap_Time_Studios
#ifndef MESSAGE_SYSTEM_H_
#define MESSAGE_SYSTEM_H_

#include <list>
using namespace std;

//some fowarding
class Message;
class GameObject;

class MessageSystem
{
public:
	// you can obtain/shutdown the instance of the singleton
	static MessageSystem* getSingleton();
	static void shutdownSingleton();

	void updateTargets(list<GameObject*>* targets);

	/*
	 *sends message to all targets
	 */
	void sendMessage(Message* msg);
	void sendMessageName(Message* msg, string name);
	void sendMessageType(Message* msg, string type);
	void sendMessageGroup(Message* msg, string group);

	void sendMessageComponentName(Message* msg, string name, string cname);
	void sendMessageComponentType(Message* msg, string name, string ctype);
	void sendMessageComponentGroup(Message* msg, string name, string cgroup);

	void sendMessageGameObject(Message* msg, GameObject* GO);

private:
	list<GameObject*>* targets_; //dynamic gamestate GO list

	//singleton patern
	static MessageSystem* instance_;
	MessageSystem();
	~MessageSystem();
};

#endif /* MESSAGE_SYSTEM_H_ */