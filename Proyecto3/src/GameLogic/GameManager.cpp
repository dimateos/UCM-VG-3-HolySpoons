#include "GameManager.h"
#include <LogSystem.h>

#include <SDL_events.h>
#include <Transforms.h>

#include "GameStateMachine.h"
#include "HPComponent.h"
#include "PhysicsControllerComponent.h"

#include "OverlayComponent.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <RenderSystemInterface.h>
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

	ScoreText->setCaption("SCORE: " + std::to_string(score));
}

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

	// hide of death panel
	overlayComp = static_cast<OverlayComponent*>(owner_->getComponent("canvas"));
	overlayComp->hidePanelByName("DEATH_PANEL");

	// player HP and score
	HPText = static_cast<TextAreaOverlayElement*>(RenderSystemInterface::getSingleton()->getOverlayElement("HP_Text"));
	ScoreText = static_cast<TextAreaOverlayElement*>(RenderSystemInterface::getSingleton()->getOverlayElement("SCORE_Text"));

	player_ = GameStateMachine::getSingleton()->currentState()->getPlayer();
	playerHP_ = static_cast<HPComponent*>(player_->getComponent("hp_component"));

	// scope
	Ogre::OverlayElement* scope = RenderSystemInterface::getSingleton()->getOverlayElement("Scope");
	RenderSystemInterface::getSingleton()->setOverlayElementCenteredPosition
	(scope, RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualWidth() / 2,
		RenderSystemInterface::getSingleton()->getCamera()->getViewport()->getActualHeight() / 2);

	updateUI();
}

void GameManager::update(GameObject * o, double time) {
	//if(playerHP_ != nullptr && playerHP_->getHP() <= 0) // EN UN FUTURO ESTO PUSEHARA UN ESTADO DE MUERTE
	//	overlayComp->showPanelByName("DEATH_PANEL");

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
	}
	else if (msg->id_ == CHECK_HP) {
		// EN UN FUTURO ESTO PUSEHARA UN ESTADO DE MUERTE
		if (playerHP_ != nullptr && playerHP_->getHP() <= 0) overlayComp->showPanelByName("DEATH_PANEL");
		updateUI();
	}
}

void GameManager::resetPlayer() {
	LogSystem::Log("RESET");
	player_->setPosition({ 0,10,0 });
	static_cast<PhysicsControllerComponent*>(player_->getComponent("controller_phy"))->setV(vO);
	playerHP_->resetHP();
}

#include "GOFactory.h"
REGISTER_TYPE(GameManager);