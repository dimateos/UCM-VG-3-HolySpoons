#pragma once
#include <OgreRoot.h>
#include <OgreBuildSettings.h>
#include <OgreRenderSystem.h>
#include "OgreRenderWindow.h"

using namespace Ogre;

class OgreInterface //: public Ogre::FrameListener
{
private:
	Ogre::Root* mRoot = nullptr;
	Ogre::RenderWindow* mWindow = nullptr;
	RenderSystem* rs = nullptr;
	void createRoot();
	bool setConfiguration();
	void createWindow();

public:
	OgreInterface(): mRoot(0) {};
	virtual ~OgreInterface() { delete mRoot; };
	void initApp();
};

