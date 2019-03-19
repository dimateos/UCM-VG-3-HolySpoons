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
	//Ogre::OverlayManager *om = new Ogre::OverlayManager();
	olm = OverlayManager::getSingletonPtr();
	if (init == 0)
	{
		panel = static_cast<OverlayContainer*>(olm->createOverlayElement("Panel", "GUI"));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(10, 10);
		panel->setDimensions(1.0f, 1.0f);
		panel->setMaterialName("DebugMaterial");
		overlay = olm->create("GUI_OVERLAY");
		overlay->add2D(panel);
	}
	++(this->init);
	szElement = "element_" + StringConverter::toString(init);
	overlay = olm->getByName("GUI_OVERLAY");
	panel = static_cast<OverlayContainer*>(olm->getOverlayElement("GUI"));
	textArea = static_cast<TextAreaOverlayElement*>(olm->createOverlayElement("TextArea", szElement));
	panel->addChild(textArea);
	overlay->show();
}
OgreText::~OgreText()
{
	szElement = "element_" + StringConverter::toString(init);
	olm->destroyOverlayElement(szElement);
	--(this->init);
	if (init == 0)
	{
		olm->destroyOverlayElement("GUI");
		olm->destroy("GUI_OVERLAY");
	}
}
/*
 *SetText with charArray
 */
/*void OgreText::setText(char *szString)
{
	textArea->setCaption(szString);
	textArea->setDimensions(1.0f, 1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	//textArea->setFontName("Srisakdi-Bold.ttf");
	textArea->setFontName("BlueHighway");
	textArea->setCharHeight(0.03f);
}*/
/*
 *SetText with Ogre::String
 */
void OgreText::setText(std::string szString) // now You can use Ogre::String as text
{
	textArea->setCaption(szString);
	textArea->setDimensions(1.0f, 1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	
	FontPtr mFont = FontManager::getSingleton().create("tutorialFont", "General");
	mFont->setType(Ogre::FT_TRUETYPE);
	mFont->setSource("Assets\Fonts\Tuto.ttf");
	mFont->setTrueTypeSize(200);
	mFont->setTrueTypeResolution(96);
	mFont->addCodePointRange(Ogre::Font::CodePointRange(33, 255));

	textArea->setFontName("tutorialFont");
	textArea->setCharHeight(3.0f);
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
void OgreText::setTextColor(float R, float G, float B, float I)
{
	textArea->setColour(Ogre::ColourValue(R, G, B, I));
}

