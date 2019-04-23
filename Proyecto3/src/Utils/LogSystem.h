#ifndef LOG_SYSTEM_H_
#define LOG_SYSTEM_H_

#include <string>
using namespace std;

#include "jsonCore.h" //easier logging of jsons

struct codePath
{
	string file;
	int line;
};
#define nonCodePath "non"
const codePath nonCode = { nonCodePath, -1 };

// System that logs messages in a .log file and in the console
class LogSystem
{
public:
	enum logType //set prefix of msg
	{
		NOTYPE, DEV, CFG, JSON, PHYS, REND, GAME, SOUND, SPAWNER, FACTORY, CONTROLLER, GM
	};

	static void resetLogFile();

	static void Log(string const & msg, logType t = NOTYPE, codePath code = nonCode);			// you can log a message
	static void Log(string const & msg, int n, logType t = NOTYPE, codePath code = nonCode);	// easier logging of an int
	static void Log(string const & msg, float n, logType t = NOTYPE, codePath code = nonCode);	// easier logging of float
	static void Log(string const & msg, double n, logType t = NOTYPE, codePath code = nonCode);	// easier logging of double
	static void Log(codePath code = nonCode);													// default dev log test msg

	static void Log(string const & msg, nap_json const & j, logType t = NOTYPE, codePath code = nonCode);	// logging json

	static void cls(size_t n = 2);				//print n empty lines
	static string getTime();					//get string with system time
	static string getCodeStuff(codePath code);	//format code file and line

private:
	//config
	static bool log_dev, log_cmd, log_file, log_time, log_code;
	static string log_route;
};

#endif /* LOG_SYSTEM_H_ */

