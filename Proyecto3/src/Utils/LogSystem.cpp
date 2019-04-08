#include "LogSystem.h"
#include <fstream>
#include <iostream>
#include <ctime>

string LogSystem::log_route = "..\\exes\\P3.log";
bool LogSystem::log_dev = true;
bool LogSystem::log_cmd = true;
bool LogSystem::log_file = true;

// when the log system is created, it destroys the last .log file
void LogSystem::resetLogFile() { remove(LogSystem::log_route.c_str()); }

// it writes the message msg in the .log file and in the console
// with the time when the message was sent. The .log file will be in "exes\"
void LogSystem::Log(string const & s, logType t) {
	string pre = "";
	switch (t) {
	default:
	case NOTYPE:
		if (!LogSystem::log_dev) return;
		pre = ":"; break;

	case CFG: pre = "CFG"; break;
	case JSON: pre = "JSON"; break;
	case PHYS: pre = "PHYS"; break;
	case REND: pre = "REND"; break;
	case GAME: pre = "GAME"; break;
	case SOUND: pre = "SOUND"; break;
	case SPAWNER: pre = "SPAWNER"; break;

	case DEV:
		if (!LogSystem::log_dev) return;
		pre = "//////////";
		break;
	}

	string msg = getTime() + pre + ": " + s + "\n";

	if (LogSystem::log_cmd) std::cout << msg;

	if (LogSystem::log_file) {
		// write file ios::app -> every output operation will be performed at the end of the file
		ofstream o (LogSystem::log_route, ios::app);
		if (o.is_open()) o << msg;
		o.close();
	}
}

void LogSystem::Log(string const & s, int n, logType t) {
	Log(s + to_string(n), t);
}
void LogSystem::Log(string const & s, float n, logType t) {
	Log(s + to_string(n), t);
}
void LogSystem::Log() {
	Log("Test point reached", DEV);
}

void LogSystem::Log(string const & msg, nlohmann::json const & j, logType t) {
	Log(msg + j.dump(), t);
}

void LogSystem::cls(size_t n) {
	for (size_t i = 0; i < n; i++) {
		if (LogSystem::log_cmd) std::cout << std::endl;
		if (LogSystem::log_file) {
			// write file ios::app -> every output operation will be performed at the end of the file
			ofstream o(LogSystem::log_route, ios::app);
			if (o.is_open()) o << std::endl;
			o.close();
		}
	}
}

string LogSystem::getTime() {
	time_t now = time(0);
	char str[26];
	ctime_s(str, sizeof str, &now);
	string time = { str[11], str[12], str[13] , str[14] ,
		str[15] , str[16] , str[17], str[18], ' ' };

	return time;
}
