#include "SoundManager.h"
#include <LogSystem.h>


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

// update of all the positions and the engine
void SoundManager::update() {
	// listener transform (position, orientation)
	updateListener();

	// updates each playing sound position
	for (map<string, pair<irrklang::ISound*, nap_vector3*>>::iterator it = threeDsounds.begin(); it != threeDsounds.end(); it++) {
		if (!it->second.first->isFinished())
			it->second.first->setPosition(irrklang::vec3df(it->second.second->x_, it->second.second->y_, it->second.second->z_));
		else threeDsounds.erase(it);
	}

	engine->update();
}

void SoundManager::updateListener()
{
	if (listenerTransform == nullptr)
		LogSystem::Log("listenerTransform hasn´t been initialized. Call singleton->getEngine->setListener at least once", LogSystem::SOUND);
	else engine->setListenerPosition(irrklang::vec3df(listenerTransform->p_.x_, listenerTransform->p_.y_, listenerTransform->p_.z_),
		irrklang::vec3df(listenerTransform->q_.x_, listenerTransform->q_.y_, listenerTransform->q_.z_)); // PARSING NEEDED

}

//Sets the position of the listener. Call this method at least once after you call getsingleton for the first time. Doesn´t need to be updated since its a pointer
void SoundManager::setListenerTransform(nap_transform* trans)
{
	listenerTransform = trans;
	updateListener();
}

// it plays a 3D sound. If the sound exists and is not playing, it will be removed before adding it again. If the tracked bool is true,
// it also returns the sound. If you play an existent sound that is being played, you will have to use this bool to have reference to the new sound
irrklang::ISound* SoundManager::play3DSound(const string& name, nap_vector3* pos, bool playLooped, bool startPaused, bool tracked)
{
	irrklang::ISound* sound3D = engine->play3D((soundsRoute + name).c_str(), irrklang::vec3df(pos->x_, pos->y_, pos->z_), playLooped, startPaused, true);
	threeDsounds.insert({ name, {sound3D, pos} });

	if (tracked)return sound3D;
	else return nullptr;
}

// it plays a 2D sound. If the sound exists and is not playing it will be removed before adding it again. If the tracked bool is true,
// it also returns the sound. If you play an existent sound that is being played, you will have to use this bool to have reference to the new sound
irrklang::ISound* SoundManager::play2DSound(const string& name, bool playLooped, bool startPaused, bool tracked)
{
	map<string, irrklang::ISound*>::iterator it = twoDsounds.find(name);
	if (it != twoDsounds.end() && it->second->isFinished())
		twoDsounds.erase(it);

	irrklang::ISound* sound2D = engine->play2D((soundsRoute + name).c_str(), playLooped, startPaused, true);
	twoDsounds.insert({ name, sound2D });

	if (tracked)return sound2D;
	else return nullptr;
}

bool SoundManager::isPlaying(const string& name) {
	return engine->isCurrentlyPlaying((soundsRoute + name).c_str());
}

// it returns the 3D sound with the name specified
irrklang::ISound* SoundManager::find3DByName(const string& name) {
	map<string, pair<irrklang::ISound*, nap_vector3*>>::iterator it = threeDsounds.find(name);
	if (it != threeDsounds.end())
		return threeDsounds.at(name).first;
	else return nullptr;
}

// it returns the 2D sound with the name specified
irrklang::ISound* SoundManager::find2DByName(const string& name) {
	map<string, irrklang::ISound*>::iterator it = twoDsounds.find(name);
	if (it != twoDsounds.end())
		return twoDsounds.at(name);
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
