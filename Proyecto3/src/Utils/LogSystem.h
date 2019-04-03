#ifndef LOG_SYSTEM_H_
#define LOG_SYSTEM_H_

#include <string>
using namespace std;

#include "json.hpp" //easier logging of jsons

// System that logs messages in a .log file and in the console
class LogSystem {
public:
	enum logType //set prefix of msg
	{
		NOTYPE, DEV, CFG, JSON, PHYS, REND, GAME, SOUND
	};

	static void resetLogFile();

	static void Log(string const & msg, logType t = NOTYPE);			// you can log a message
	static void Log(string const & msg, int n, logType t = NOTYPE);		// easier logging of an int
	static void Log(string const & msg, float n, logType t = NOTYPE);	// easier logging of float
	static void Log();													// default dev log test msg

	static void Log(string const & msg, nlohmann::json const & j, logType t = NOTYPE);	// logging json

	static void cls(size_t n = 2);	//print n empty lines
	static string getTime();		//get string with system time

private:
	//config
	static bool log_dev, log_cmd, log_file;
	static string log_route;
};

#endif /* LOG_SYSTEM_H_ */

