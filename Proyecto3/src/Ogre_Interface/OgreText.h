//-----------------------------------------------------------------------------
// Lonewolff
//
// Filename:    OgreText.h
// Description: Class for simple text in Ogre (Version 040507:18.30)
// http://wiki.ogre3d.org/Simple+text
//-----------------------------------------------------------------------------

#include "RenderSystemInterface.h"

namespace Ogre {
	class OverlayManager;
	class OverlayContainer;
	class Overlay;
	class TextAreaOverlayElement;
}

#include <string>

#ifndef __OgreText_H__
#define __OgreText_H__

class OgreText
{
private:
	Ogre::OverlayManager *overlayManager;
	Ogre::OverlayContainer *panel;
	Ogre::Overlay *overlay;
	Ogre::TextAreaOverlayElement *textArea;
	static int init;
	std::string szElement;

public:
	OgreText();
	~OgreText();
	/*
	 *SetText with charArray
	 */ 
	//void setText(char *szString);
	/*
	 *SetText with Ogre::String
	 */
	void setText(std::string szString); // now You can use Ogre::String as text
	/*
	 *Set text position on overlay
	 */
	void setTextPosition(float x, float y);
	/*
	 *Set text color
	 */
	void setTextColour(float R, float G, float B, float I);
	void setTextColourTop(float R, float G, float B, float I);
	void setTextColourBot(float R, float G, float B, float I);
};

#endif