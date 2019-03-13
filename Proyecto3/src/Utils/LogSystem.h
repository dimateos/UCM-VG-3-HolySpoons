#ifndef LOG_SYSTEM_H_
#define LOG_SYSTEM_H_

#include <string>
using namespace std;

// System that logs messages in a .log file and in the console
class LogSystem {
private:
	static LogSystem* instance_; //singleton pattern

	LogSystem() {}
	virtual ~LogSystem() {}

public:
	static LogSystem* getSingleton();   // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();
	static void Log(const string& msg); // you can log a message
};

#endif /* LOG_SYSTEM_H_ */

