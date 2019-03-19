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
			overlayManager->createOverlayElement("Panel", "PanelName"));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(10, 10);
		panel->setDimensions(100, 100);
		//panel->setMaterialName("MaterialName"); // Optional background material
		// Create an overlay, and add the panel
		Overlay* overlay = overlayManager->create("OverlayName");
		overlay->add2D(panel);
	}
	++(this->init);

	szElement = "element_" + StringConverter::toString(init);
	overlay = overlayManager->getByName("OverlayName");
	panel = static_cast<OverlayContainer*>(overlayManager->getOverlayElement("PanelName"));
	// Create a text area
	textArea = static_cast<TextAreaOverlayElement*>(
		overlayManager->createOverlayElement("TextArea", szElement));
	textArea->setMetricsMode(Ogre::GMM_PIXELS);
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
		overlayManager->destroyOverlayElement("PanelName");
		overlayManager->destroy("OverlayName");
	}
}

/*
 *SetText with Ogre::String
 */
void OgreText::setText(std::string szString) // now You can use Ogre::String as text
{
	textArea->setCaption(szString);
	textArea->setCharHeight(30);
	textArea->setFontName("MyFont");
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

void OgreText::setTextColourTop(float R, float G, float B, float I)
{
	textArea->setColourTop(Ogre::ColourValue(R, G, B, I));
}

void OgreText::setTextColourBot(float R, float G, float B, float I)
{
	textArea->setColourBottom(Ogre::ColourValue(R, G, B, I));
}

