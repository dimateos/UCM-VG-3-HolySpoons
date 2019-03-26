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

irrklang::ISound* SoundManager::play3DSound(const char * soundFileName, float x, float y, float z, bool playLooped, bool startPaused, bool track)
{
	return engine->play3D((soundsRoute + soundFileName).c_str(), irrklang::vec3df(x,y,z), playLooped, startPaused, track);
}

irrklang::ISound * SoundManager::play2DSound(const char * soundFileName, bool playLooped, bool startPaused, bool track)
{
	return engine->play2D((soundsRoute + soundFileName).c_str(), playLooped, startPaused, track);
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
	engine = nullptr;
}
