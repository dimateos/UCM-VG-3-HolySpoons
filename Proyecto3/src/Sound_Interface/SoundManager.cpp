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

	// 3Dsounds: updates each playing sound position and erases the stopped sounds
	for (map<string, pair<irrklang::ISound*, nap_vector3*>>::iterator it = threeDsounds.begin(); it != threeDsounds.end(); it++) {
		if (!it->second.first->isFinished())
			it->second.first->setPosition(irrklang::vec3df(it->second.second->x_, it->second.second->y_, it->second.second->z_));
		else threeDsounds.erase(it);
	}
	// 2Dsounds: erases the stopped sounds
	for (map<string, irrklang::ISound*>::iterator it = twoDsounds.begin(); it != twoDsounds.end(); it++) {
		if (!it->second->isFinished());
		else twoDsounds.erase(it);
	}

	engine->update();
}

void SoundManager::updateListener()
{
	if (listenerTransform == nullptr)
		LogSystem::Log("listenerTransform hasn´t been initialized. Call singleton->getEngine->setListener at least once", LogSystem::SOUND);
	else {
		nap_vector3 dir = listenerTransform->q_.toNapVec3(nap_vector3(0, 0, 1));

		engine->setListenerPosition(irrklang::vec3df(listenerTransform->p_.x_, listenerTransform->p_.y_, listenerTransform->p_.z_),
			irrklang::vec3df(dir.x_, dir.y_, dir.z_)); // PARSING NEEDED
	}

}

//Sets the position of the listener. Call this method at least once after you call getsingleton for the first time. Doesn´t need to be updated since its a pointer
void SoundManager::setListenerTransform(nap_transform* trans)
{
	listenerTransform = trans;
	updateListener();
}

// it plays a 3D sound. If the track bool is true, it also returns the sound.
// If you want to change a sound in execution, you must name it to have a later reference
irrklang::ISound* SoundManager::play3DSound(const string& routeName, nap_vector3* pos, bool playLooped, bool startPaused, string customName, bool track)
{
	irrklang::ISound* sound3D = engine->play3D((soundsRoute + routeName).c_str(), irrklang::vec3df(pos->x_, pos->y_, pos->z_), playLooped, startPaused, true);
	if(customName != "") threeDsounds.insert({ customName, {sound3D, pos} }); // you will be able to have a reference to that sound later
	else {
		threeDsounds.insert({ "__" + to_string(unmodifiedSounds), {sound3D, pos} });
		unmodifiedSounds++;
	}

	if (track)return sound3D;
	else return nullptr;
}

// it plays a 2D sound. If the track bool is true, it also returns the sound.
// If you want to change a sound in execution, you must name it to have a later reference
irrklang::ISound* SoundManager::play2DSound(const string& routeName, bool playLooped, bool startPaused, string customName, bool track)
{
	irrklang::ISound* sound2D = engine->play2D((soundsRoute + routeName).c_str(), playLooped, startPaused, true);
	if (customName != "") twoDsounds.insert({ customName, sound2D }); // you will be able to have a reference to that sound later
	else {
		twoDsounds.insert({ "__" + to_string(unmodifiedSounds), sound2D });
		unmodifiedSounds++;
	}

	if (track)return sound2D;
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
