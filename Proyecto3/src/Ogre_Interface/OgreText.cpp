#include "OgreText.h"
#include <OgreTextAreaOverlayElement.h>
#include "OgreStringConverter.h"
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>
#include <OgreFontManager.h>

using namespace Ogre;

int OgreText::init = 0;

OgreText::OgreText()
{
	overlayManager = OverlayManager::getSingletonPtr();

	if (init == 0) {
		// Create a panel
		OverlayContainer* panel = static_cast<OverlayContainer*>(
			overlayManager->createOverlayElement("Panel", panelName));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(10, 10);
		panel->setDimensions(100, 100);
		// Create an overlay, and add the panel
		Overlay* overlay = overlayManager->create(overlayName);
		overlay->add2D(panel);
	}
	++(this->init);

	szElement = "element_" + StringConverter::toString(init); //Element name
	overlay = overlayManager->getByName(overlayName); //We ge the Overlay by name
	panel = static_cast<OverlayContainer*>(overlayManager->getOverlayElement(panelName));
	// Create a text area
	textArea = static_cast<TextAreaOverlayElement*>(
		overlayManager->createOverlayElement("TextArea", szElement));
	textArea->setMetricsMode(Ogre::GMM_PIXELS); //Maybe RELATIVE ????

	textArea->setColourBottom(ColourValue(0.3, 0.5, 0.3));
	textArea->setColourTop(ColourValue(0.5, 0.7, 0.5));
	// Add the text area to the panel
	panel->addChild(textArea);

	// Show the overlay
	overlay->show();
}
OgreText::~OgreText()
{
	szElement = "element_" + StringConverter::toString(init);
	overlayManager->destroyOverlayElement(szElement);
	--(this->init);
	if (init == 0)
	{
		overlayManager->destroyOverlayElement(panelName);
		overlayManager->destroy(overlayName);
	}
}

/*
 *SetText with Ogre::String
 */
void OgreText::setText(std::string szString) // now You can use Ogre::String as text
{
	textArea->setCaption(szString);
	textArea->setCharHeight(30);
	textArea->setFontName("HackReg");
	textArea->setDimensions(100, 100);
}
/*
 *Set text position on overlay
 */
void OgreText::setTextPosition(float x, float y)
{
	textArea->setPosition(x, y);
}

/*
 *Set text color
 */

void OgreText::setTextColour(float R, float G, float B, float I)
{
	textArea->setColour(Ogre::ColourValue(R, G, B, I));
}
/*
 *Set Top-font color
 */
void OgreText::setTextColourTop(float R, float G, float B, float I)
{
	textArea->setColourTop(Ogre::ColourValue(R, G, B, I));
}
/*
 *Set Dropshadow color
 */
void OgreText::setTextColourBot(float R, float G, float B, float I)
{
	textArea->setColourBottom(Ogre::ColourValue(R, G, B, I));
}

