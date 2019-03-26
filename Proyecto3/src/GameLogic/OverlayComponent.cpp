#include "OverlayComponent.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>

void OverlayComponent::setUp(nap_json const & cfg)
{
	overlay = Ogre::OverlayManager::getSingleton().getByName(cfg["overlay_name"]);
	panel = overlay->getChild(cfg["panel_container"]);
	// Show the overlay
	showOverlay();
}

void OverlayComponent::setDown()
{
	Ogre::OverlayManager::getSingletonPtr()->destroyOverlayElement(panel);
	Ogre::OverlayManager::getSingletonPtr()->destroy(overlay);
}

void OverlayComponent::hideOverlay()
{
	overlay->hide();
}

void OverlayComponent::showOverlay()
{
	overlay->show();
}
