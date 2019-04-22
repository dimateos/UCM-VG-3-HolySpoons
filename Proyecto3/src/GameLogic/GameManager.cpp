#include "GameManager.h"
#include "GOFactory.h"
#include "OverlayComponent.h"
#include "GameStateMachine.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <RenderSystemInterface.h>

using namespace Ogre;

void GameManager::updateUI()
{
	//HPText->setCaption("SCORE: " + std::to_string(playerLife));
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

	// scope
	Ogre::OverlayElement* scope = RenderSystemInterface::getSingleton()->getOverlayElement("Scope");
	RenderSystemInterface::getSingleton()->setOverlayElementCenteredPosition(scope, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	updateUI();
}

void GameManager::update(GameObject * o, double time) {
	// update of the HP HUD with the player HP
	// checks if the player HP is under 0 (DEAD panel)
	// or if the player has completed the round (NEXT ROUND panel)
}

void GameManager::receive(Message * msg)
{
	if (msg->id_ == ADD_SCORE) {
		addScore(static_cast<Msg_ADD_SCORE*>(msg)->score_);
		updateUI();
	}
}


REGISTER_TYPE(GameManager);