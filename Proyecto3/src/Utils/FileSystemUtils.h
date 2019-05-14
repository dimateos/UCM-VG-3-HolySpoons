#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "LogSystem.h"
#include <string>
#include <list>

#include <direct.h>
#include "dirent.h"
//read all directory files using dirent.h as filesystem.h is c++ 17

// Some filesystem utilities
class FileSystemUtils
{
public:
	//create a folder from the apps directory (only if it doesnt exist already)
	static void createFolderOnCWD(std::string fname) {
		char cwd[FILENAME_MAX];
		_getcwd(cwd, sizeof(cwd));
		//printf("The current working directory is %s", cwd);

		std::string f = cwd;
		f += "\\" + fname;

		int stat = _mkdir(f.c_str());
		if (!stat) LogSystem::Log("Directorio " + fname + " creado...", LogSystem::JSON);
		else LogSystem::Log("No se pudo crear el directorio " + fname + " (ya existia?)", LogSystem::JSON);
	}

	//returns a list with the paths to each of the entries found in given dirpath
	static std::list<std::string> retrieveDirEntries(std::string dirpath) {
		std::list<std::string> names;

		DIR *dir;
		struct dirent *ent;
		if ((dir = opendir(dirpath.c_str())) == NULL) {
			LogSystem::Log(dirpath + " no fue encontrado... abortando retrieveDirEntries", LogSystem::JSON);
			return names;
		}

		//ignore . and ..
		readdir(dir); readdir(dir);

		//list one by one
		while ((ent = readdir(dir)) != NULL) {
			std::string s = ent->d_name;
			names.push_back(dirpath + "\\" + s);
		}

		LogSystem::Log("Encontradas " + to_string(names.size()) + " entradas en " + dirpath, LogSystem::JSON);
		closedir(dir);
		return names;
	}
};

#endif /* FILESYSTEM_H_ */

