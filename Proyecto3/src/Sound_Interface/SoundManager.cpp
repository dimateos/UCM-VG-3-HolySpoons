#include "SoundManager.h"
#include <irrKlang.h>

using namespace irrklang;

SoundManager* SoundManager::instance_ = nullptr;

SoundManager* SoundManager::getSingleton() {
	if (instance_ == nullptr) {
		instance_ = new SoundManager();
	}

	return instance_;
}

void SoundManager::shutdownSingleton() {
	if (instance_ == nullptr) return;

	delete instance_;
	instance_ = nullptr;
}

// it plays a 3D sound. If the sound exists and is not playing, it will be removed before adding it again. If the tracked bool is true,
// it also returns the sound. If you play an existent sound that is being played, you will have to use this bool to have reference to the new sound
irrklang::ISound* SoundManager::play3DSound(const string& name, float x, float y, float z, bool playLooped, bool startPaused, bool tracked)
{
	map<string, ISound*>::iterator it = sounds.find(name);
	if (it != sounds.end() && it->second->isFinished())
		sounds.erase(it);

	irrklang::ISound* sound3D = engine->play3D((soundsRoute + name).c_str(), irrklang::vec3df(x, y, z), playLooped, startPaused, true);
	sounds.insert({ name, sound3D });

	if (tracked)return sound3D;
	else return nullptr;
}

// it plays a 2D sound. If the sound exists and is not playing it will be removed before adding it again. If the tracked bool is true,
// it also returns the sound. If you play an existent sound that is being played, you will have to use this bool to have reference to the new sound
irrklang::ISound* SoundManager::play2DSound(const string& name, bool playLooped, bool startPaused, bool tracked)
{
	map<string, ISound*>::iterator it = sounds.find(name);
	if (it != sounds.end() && it->second->isFinished())
		sounds.erase(it);

	irrklang::ISound* sound2D = engine->play2D((soundsRoute + name).c_str(), playLooped, startPaused, true);
	sounds.insert({ name, sound2D });

	if (tracked)return sound2D;
	else return nullptr;
}

bool SoundManager::isPlaying(const string& name) {
	return engine->isCurrentlyPlaying((soundsRoute + name).c_str());
}

// it returns the sound with the name specified
irrklang::ISound* SoundManager::findByName(const string& name) {
	map<string, ISound*>::iterator it = sounds.find(name);
	if (it != sounds.end())
		return sounds.at(name);
	else return nullptr;
}

irrklang::ISoundEngine * SoundManager::getEngine()
{
	return engine;
}

SoundManager::SoundManager():engine(createIrrKlangDevice())
{
}

SoundManager::~SoundManager()
{
	engine->drop();  // close of the engine	
}
