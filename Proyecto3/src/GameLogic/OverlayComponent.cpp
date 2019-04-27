#include "OverlayComponent.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>
#include <RenderSystemInterface.h>
#include "GOFactory.h"

void OverlayComponent::setUp()
{
	overlay = Ogre::OverlayManager::getSingleton().getByName(cfg_["overlay_name"]);
	panel = overlay->getChild(cfg_["panel_container"]);
}

void OverlayComponent::lateSetUp()
{
	// Show the overlay
	showOverlay();
}

void OverlayComponent::setDown()
{
	hideOverlay();
}

void OverlayComponent::hideOverlay()
{
	overlay->hide();
}

void OverlayComponent::showOverlay()
{
	overlay->show();
}

Ogre::OverlayElement * OverlayComponent::getOverlayElementByName(std::string name)
{
	return overlay->getChild(name);
}

void OverlayComponent::hidePanelByName(std::string name)
{
	getOverlayElementByName(name)->hide();
}

void OverlayComponent::showPanelByName(std::string name)
{
	getOverlayElementByName(name)->show();
}

void OverlayComponent::setPanelAlpha(float alpha)
{
	Ogre::ColourValue color = panel->getColour();
	color.a = alpha;
	panel->setColour(color);
}

void OverlayComponent::receive(Message * msg)
{
	if (msg->id_ == STATE_CHANGED) {
		overlay->show();
	}
}

REGISTER_TYPE(OverlayComponent);
