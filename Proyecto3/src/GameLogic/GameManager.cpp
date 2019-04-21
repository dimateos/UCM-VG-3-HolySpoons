#include "GameManager.h"
#include "GOFactory.h"
#include "OverlayComponent.h"
#include "GameStateMachine.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayContainer.h>

using namespace Ogre;

void GameManager::setUp() {
	if (isInited()) return;
	setInited();

	overlayComp = static_cast<OverlayComponent*>(owner_->getComponent("canvas"));
	overlayComp->hidePanelByName("DEATH_PANEL");

	OverlayContainer* panel = static_cast<OverlayContainer*>(overlayComp->getOverlayElementByName("HUD_PanelContainer"));
	HPText = static_cast<TextAreaOverlayElement*>(panel->getChild("HP_Text"));
	ScoreText = static_cast<TextAreaOverlayElement*>(panel->getChild("SCORE_Text"));

	player_ = GameStateMachine::getSingleton()->currentState()->getPlayer();

	ScoreText->setCaption("SCORE: " + std::to_string(score));
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
		ScoreText->setCaption("SCORE: " + std::to_string(score));
	}
}


REGISTER_TYPE(GameManager);