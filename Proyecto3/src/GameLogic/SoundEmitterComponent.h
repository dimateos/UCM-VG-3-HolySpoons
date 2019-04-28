//Nap_Time_Studios
#ifndef SOUND_EMITTER_COMP_H_
#define SOUND_EMITTER_COMP_H_

#include "Component.h"

namespace irrklang {
	class ISound;
}

// it allows its owner to play a sound
class SoundEmitterComponent : public Component
{
private:
	string routeName;
	string customName;         // custom name for searching it in the sounds map (SoundManager)

	nap_transform* emitterPos; // only if it is a 3D sound

	bool threeD;
	bool playLooped;
	bool startPaused;
	bool track;

	float volume;

	irrklang::ISound* sound;

	// some methods
	void playSound();
	void stopSound();
	void setVolume(float v);

public:
	inline SoundEmitterComponent(nap_json const & cfg, GameObject* owner) : Component(cfg, owner) {};
	inline virtual ~SoundEmitterComponent() { };
	virtual void setUp();

	virtual void receive(Message * msg);
};

#endif /* SOUND_EMITTER_COMP_H_ */
