#include "GameManager.h"
#include <LogSystem.h>

#include <SDL_events.h>
#include <Transforms.h>

#include "MessageSystem.h"
#include "GameStateMachine.h"
#include "HPComponent.h"
#include "PhysicsControllerComponent.h"

#include "OverlayComponent.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <RenderSystemInterface.h>
#include <SoundManager.h>
#include <LogSystem.h>
#include <OgreCamera.h>
#include <OgreViewport.h>

using namespace Ogre;

// updates player HP and score information
void GameManager::updateUI()
{
	if(playerHP_ != nullptr)
		HPText->setCaption("HP: " + std::to_string(playerHP_->getHP()));
	else
		LogSystem::Log("Componente HPComponent no establecido. No es posible mostrar HP", LogSystem::GM);

	ScoreText->setCaption("SCORE: " + std::to_string(score_));
}

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

	string s = this->cfg_["state"], j = this->cfg_["json"];
	state = s, json = j;

	auto rsi = RenderSystemInterface::getSingleton();

	// hide of death panel
	overlayComp = static_cast<OverlayComponent*>(owner_->getComponent("canvas"));
	overlayComp->hidePanelByName("HIT_MARKER_PANEL");
	overlayComp->hidePanelByName("DEATH_MARKER_PANEL");

	// player HP and score
	HPText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("HP_Text"));
	ScoreText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("SCORE_Text"));

	player_ = GameStateMachine::getSingleton()->currentState()->getPlayer();
	playerHP_ = static_cast<HPComponent*>(player_->getComponent("hp_component"));

	// scope
	Ogre::OverlayElement* scope = rsi->getOverlayElement("Scope");
	rsi->setOverlayElementCenteredPosition_rel(scope, 0.5, 0.5);

	// hitMarker
	Ogre::OverlayElement* hitMarker = rsi->getOverlayElement("HitMarker");
	rsi->setOverlayElementCenteredPosition_rel(hitMarker, 0.5, 0.5);

	// deathMarker
	Ogre::OverlayElement* deathMarker = rsi->getOverlayElement("DeathMarker");
	rsi->setOverlayElementCenteredPosition_rel(deathMarker, 0.5, 0.5);

	updateUI();
}

void GameManager::update(GameObject * o, double time) {
	if (timer.update(time)) {
		overlayComp->hidePanelByName("HIT_MARKER_PANEL");   // enemy damage -> hit marker (white)
		overlayComp->hidePanelByName("DEATH_MARKER_PANEL"); // enemy death -> death marker (red)
	}

	// or if the player has completed the round (NEXT ROUND state)
}

bool GameManager::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		//HAXS
#if _DEBUG
		if (pressedKey == SDLK_r) {
			resetPlayer();
			handled = true;
		}
#endif
	}

	return handled;
}

void GameManager::receive(Message * msg)
{
	if (msg->id_ == ADD_SCORE) {
		addScore(static_cast<Msg_ADD_SCORE*>(msg)->score_);
		updateUI();
		overlayComp->showPanelByName("DEATH_MARKER_PANEL"); // enemy death -> death marker (red)
		timer.setDuration(0.3);
		timer.start();
	}
	else if (msg->id_ == ENEMY_DAMAGE) {
		overlayComp->showPanelByName("HIT_MARKER_PANEL");   // enemy damage -> hit marker (white)
		timer.setDuration(0.1);
		timer.start();
		MessageSystem::getSingleton()->sendMessageComponentName(&Message(PLAY_SOUND), owner_->id().name_, "hitSound");
	}
	else if (msg->id_ == CHECK_HP) {
		if (playerHP_ != nullptr && playerHP_->getHP() <= 0)
			MessageSystem::getSingleton()->sendMessageComponentName(&Message(PUSH_STATE), owner_->idPtr()->name_, "push_death");
		updateUI();
	}
}

void GameManager::resetPlayer() {
	LogSystem::Log("RESET");
	player_->setPosition({ 0,10,0 });
	auto pc = static_cast<PhysicsControllerComponent*>(player_->getComponent("controller_phy"));
	pc->setV(vO);
	pc->invalidateChache();

	playerHP_->resetHP();
	score_ = 0;
	updateUI();

	MessageSystem::getSingleton()->sendMessage(&Message(RESET_PULL));
}

#include "GOFactory.h"
REGISTER_TYPE(GameManager);