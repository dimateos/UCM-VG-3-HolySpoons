#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <string>
#include <conioIRR.h>
#include <stdio.h>
#include <irrKlang.h>

using namespace std;
using namespace irrklang;

// interface for irrklang
class SoundManager
{
private:
	static SoundManager* instance_; //singleton pattern
	const string soundsRoute = "..\\exes\\Assets\\Sound\\";

	irrklang::ISoundEngine* engine; // it plays the sounds, etc

	SoundManager();
	virtual ~SoundManager();
public:
	static SoundManager* getSingleton(); // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();

	irrklang::ISoundEngine* getEngine();
};

#endif /* SOUND_MANAGER_H_ */

