#ifndef SOUND_MANAGER_H_
#define SOUND_MANAGER_H_

#include <string>
#include <map>
using namespace std;

class nap_vector3;
class nap_transform;

namespace irrklang {
	class ISound;
	class ISoundEngine;
}

// interface for irrklang (SoundManager)
class SoundManager
{
private:
	static SoundManager* instance_;                                       //singleton pattern
	const string soundsRoute = ".\\Assets\\Sound\\";
	int unmodifiedSounds = 0;                                             // number of sounds with no specific name
	std::map<string, pair<irrklang::ISound*, nap_vector3*>> threeDsounds; // 3Dsounds already played, with their position
	std::map<string, irrklang::ISound*> twoDsounds;                       // 2Dsounds already played

	irrklang::ISoundEngine* engine;                                       // it plays the sounds, etc
	nap_transform* listenerTransform;                                     // the transform of the listener (player)

	SoundManager();
	virtual ~SoundManager();

	void updateListener();

public:
	static SoundManager* getSingleton(); // you can obtain/shutdown the instance of the singleton
	static void shutdownSingleton();

	void update();

	void setListenerTransform(nap_transform* trans);       //ESPERAR A QUE DIEGO HAGA LA CONVERSION A VEC3

	// playing 3D/2D sounds... (3D will need a pointer to the emitter position
	irrklang::ISound* play3DSound(const string& routeName, nap_vector3* pos,
		bool playLooped = false, bool startPaused = false, string customName = "", bool track = false);

	irrklang::ISound* play2DSound(const string& routeName,
		bool playLooped = false, bool startPaused = false, string customName = "", bool track = false);

	bool isPlaying(const string& name);

	irrklang::ISound* find3DByName(const string& name);
	irrklang::ISound* find2DByName(const string& name);
	void stopSounds();

	irrklang::ISoundEngine* getEngine();
	void setVolume(float v);
};

#endif /* SOUND_MANAGER_H_ */

