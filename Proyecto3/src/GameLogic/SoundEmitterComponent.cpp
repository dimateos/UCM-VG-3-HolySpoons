#include "SoundEmitterComponent.h"

#include <SoundManager.h>
#include <irrKlang.h>

#include "Messages.h"

void SoundEmitterComponent::playSound()
{
	if (threeD)
		sound = SoundManager::getSingleton()->play3DSound(routeName, &emitterPos->p_, playLooped, false, customName, true);
	else
		sound = SoundManager::getSingleton()->play2DSound(routeName, playLooped, false, customName, true);

	setVolume(volume);
}

void SoundEmitterComponent::stopSound()
{
	sound->stop();
}

void SoundEmitterComponent::setVolume(float v)
{
	sound->setVolume(v);
}

void SoundEmitterComponent::setUp() {
	if (isInited()) return;
	setInited();

	if (FIND(cfg_, "routeName")) { string auxRouteName = cfg_["routeName"]; routeName = auxRouteName; }
	else routeName = "ophelia.mp3";

	if (FIND(cfg_, "customName")) { string auxCustomName = cfg_["customName"];  customName = auxCustomName; }
	else customName = "";

	if(FIND(cfg_, "3D")) threeD = cfg_["3D"];
	else threeD = false;

	if (threeD) {
		emitterPos = owner_->getTransPtr();
	}

	if (FIND(cfg_, "loop")) playLooped = cfg_["loop"];
	else playLooped = true;

	if (FIND(cfg_, "startPaused")) startPaused = cfg_["startPaused"];
	else startPaused = false;

	if (FIND(cfg_, "volume")) volume = cfg_["volume"];
	else volume = 1;
}

void SoundEmitterComponent::receive(Message * msg)
{
	if (msg->id_ == PLAY_SOUND) {
		playSound();
	}
	else if (msg->id_ == STOP_SOUND) {
		stopSound();
	}
	else if (msg->id_ == STATE_CHANGED && !startPaused) {
		playSound();
	}
}



#include "GOFactory.h"
REGISTER_TYPE(SoundEmitterComponent);