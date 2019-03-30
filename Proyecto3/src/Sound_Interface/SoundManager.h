#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <string>
#include <map>
using namespace std;
#include "checkML.h"

namespace irrklang {
	class ISoundEngine;
	class ISound;
}

// interface for irrklang
class SoundManager
{
private:
	static SoundManager* instance_; //singleton pattern
	const string soundsRoute = ".\\Assets\\Sound\\";
	std::map<string, irrklang::ISound*> sounds;

	irrklang::ISoundEngine* engine; // it plays the sounds, etc

	SoundManager();
	virtual ~SoundManager();
public:
	static SoundManager* getSingleton(); // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();

	void play3DSound(const string& name, float x, float y, float z,  //USAR UTILS
		bool playLooped = false,
		bool startPaused = false);

	void play2DSound(const string& name, 
		bool playLooped = false,
		bool startPaused = false);

	bool isPlaying(const string& name);

	irrklang::ISound* findeByName(const string& name);

	irrklang::ISoundEngine* getEngine();
};

#endif /* SOUND_MANAGER_H_ */

