#include "SoundManager.h"
#include <irrKlang.h>
#include "checkML.h"

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
