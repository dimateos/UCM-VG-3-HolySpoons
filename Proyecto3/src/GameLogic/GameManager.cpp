#include "GameManager.h"
#include <LogSystem.h>
#include <GlobalConfig.h>

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
	RoundText->setCaption("ROUND: " + std::to_string(round_));
	MiniRoundText->setCaption(std::to_string(round_));
}

void GameManager::nextRound()
{
	round_++;
	updateUI();

	overlayComp->showPanelByName("ROUND_PANEL");
	roundTimer.setDuration(roundTime);
	roundTimer.start();

	// destructible spawners -> reactive them
	MessageSystem::getSingleton()->sendMessageGroup(&Message(ACTIVE_SPAWNER), "destructible_spawner");
	// indestructuble spawners -> set their new enemies number to spawn (depending on the round)
	MessageSystem::getSingleton()->sendMessageGroup(&Msg_RESET_SPAWNER(round_), "indestructible_spawner");
}

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

	// name of the component that pushes the death state
	string state = this->cfg_["death_state"];
	death_state = state;

	// name of the component that will play the hit marker sound effect
	string sound = this->cfg_["hitMarker_sound"];
	hitMarker_sound = sound;

	auto rsi = RenderSystemInterface::getSingleton();

	// hide of death panel
	overlayComp = static_cast<OverlayComponent*>(owner_->getComponent("canvas"));
	overlayComp->hidePanelByName("HIT_MARKER_PANEL");
	overlayComp->hidePanelByName("DEATH_MARKER_PANEL");
	overlayComp->hidePanelByName("ROUND_PANEL");

	// player HP and score
	HPText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("HP_Text"));
	ScoreText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("SCORE_Text"));
	RoundText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("ROUND_Text"));
	MiniRoundText = static_cast<TextAreaOverlayElement*>(rsi->getOverlayElement("MINI_ROUND_Text"));

	player_ = GameStateMachine::getSingleton()->currentState()->getPlayer();
	playerHP_ = static_cast<HPComponent*>(player_->getComponent("hp_component"));

	// scope
	Ogre::OverlayElement* scope = rsi->getOverlayElement("Scope");
	rsi->setOverlayElementCenteredPosition_rel(scope, 0.5, 0.5);

	// hitMarker
	Ogre::OverlayElement* hitMarker = rsi->getOverlayElement("HitMarker");
	rsi->setOverlayElementCenteredPosition_rel(hitMarker, 0.5, 0.5);
	hitTime = cfg_["hitTime"]; // hit marker duration

	// deathMarker
	Ogre::OverlayElement* deathMarker = rsi->getOverlayElement("DeathMarker");
	rsi->setOverlayElementCenteredPosition_rel(deathMarker, 0.5, 0.5);
	deathTime = cfg_["deathTime"]; // death marker duration

	roundTime = cfg_["roundTime"]; // round UI duration

}

void GameManager::lateSetUp()
{
	nextRound(); // round 1
}

void GameManager::update(GameObject * o, double time) {
	MiniRoundText->setCaption(std::to_string(enemies_));			//DEBUG

	if (hitTimer.update(time)) {
		overlayComp->hidePanelByName("HIT_MARKER_PANEL");   // enemy damage -> hit marker (white)
		overlayComp->hidePanelByName("DEATH_MARKER_PANEL"); // enemy death -> death marker (red)
	}

	if(roundTimer.update(time))overlayComp->hidePanelByName("ROUND_PANEL");
}

bool GameManager::handleEvents(GameObject * o, const SDL_Event & evt) {
	bool handled = false;

#if _DEBUG //HAXS
	if (evt.type == SDL_KEYDOWN) {
		SDL_Keycode pressedKey = evt.key.keysym.sym;

		if (pressedKey == SDLK_r) {
			resetPlayer();
			handled = true;
		}
	}

#endif
	return handled;
}

void GameManager::receive(Message * msg)
{
	if (msg->id_ == BULLET_HIT) {
		Msg_BULLET_HIT* aux = static_cast<Msg_BULLET_HIT*>(msg); // object hit by a bullet
		addScore(aux->score_);
		updateUI();
		overlayComp->showPanelByName("DEATH_MARKER_PANEL"); // enemy death -> death marker (red)
		hitTimer.setDuration(deathTime);
		hitTimer.start();
		if (aux->enemy_) { // if the object was an enemy
			enemies_--;
			if (enemies_ == 0)nextRound(); // if you kill all the enemies -> next round
		}
	}
	else if (msg->id_ == ENEMY_DAMAGE) {
		overlayComp->showPanelByName("HIT_MARKER_PANEL");   // enemy damage -> hit marker (white)
		hitTimer.setDuration(hitTime);
		hitTimer.start();
		MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message(PLAY_SOUND), owner_, hitMarker_sound);
	}
	else if (msg->id_ == CHECK_HP) {
		// gm has two pushComponents (pause and death), so we need to specify which one will push its state
		if (playerHP_ != nullptr && playerHP_->getHP() <= 0) { // in this case (HP == 0) -> death state
			MessageSystem::getSingleton()->sendMessageGroup(&Msg_PLAYER_DEAD(GlobalCFG::name, this->score_), "leaderBoard");
			MessageSystem::getSingleton()->sendMessageGameObjectComponentName(&Message(PUSH_STATE), owner_, death_state);
		}
		updateUI();
	}
	else if (msg->id_ == ADD_ENEMY) { // when a spawner spawns another enemy, you will have to kill him
		enemies_ += static_cast<Msg_ADD_ENEMY*>(msg)->num_;
	}
}

void GameManager::resetPlayer() {
	LogSystem::Log("RESET");

	//reset the p controller
	player_->setPosition({ 0,10,0 });
	auto pc = static_cast<PhysicsControllerComponent*>(player_->getComponent("controller_phy"));
	pc->setV(vO);
	pc->invalidateChache();

	score_ = 0;
	updateUI();

	MessageSystem::getSingleton()->sendMessage(&Message(RESET_HP));
	MessageSystem::getSingleton()->sendMessage(&Message(RESET_PULL));
}

#include "GOFactory.h"
REGISTER_TYPE(GameManager);