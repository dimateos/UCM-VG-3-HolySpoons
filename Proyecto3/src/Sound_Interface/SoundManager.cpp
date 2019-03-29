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

void SoundManager::play3DSound(const string& name, float x, float y, float z, bool playLooped, bool startPaused)
{
	sounds.insert({ name, engine->play3D((soundsRoute + name).c_str(), irrklang::vec3df(x,y,z), playLooped, startPaused, true) });
}

void SoundManager::play2DSound(const string& name, bool playLooped, bool startPaused)
{
	sounds.insert({ name, engine->play2D((soundsRoute + name).c_str(), playLooped, startPaused, true) });
}

bool SoundManager::isPlaying(const string& name) {
	return engine->isCurrentlyPlaying((soundsRoute + name).c_str());
}

irrklang::ISound* SoundManager::findeByName(const string& name) {
	return sounds.at(name);
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
	for (map<string, irrklang::ISound*>::iterator it = sounds.begin(); it != sounds.end(); it++) {
		it->second->drop();
	}

	engine->drop();  // close of the engine	
}
