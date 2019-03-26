#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <string>
using namespace std;

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

	irrklang::ISoundEngine* engine; // it plays the sounds, etc

	SoundManager();
	virtual ~SoundManager();
public:
	static SoundManager* getSingleton(); // you can obtain/shutdown the instance of the singleton 
	static void shutdownSingleton();

	irrklang::ISound* play3DSound(const char* soundFileName, float x, float y, float z,  //USAR UTILS
		bool playLooped = false,
		bool startPaused = false,
		bool track = true);

	irrklang::ISound* play2DSound(const char* soundFileName, 
		bool playLooped = false,
		bool startPaused = false,
		bool track = true);

	irrklang::ISoundEngine* getEngine();
};

#endif /* SOUND_MANAGER_H_ */

