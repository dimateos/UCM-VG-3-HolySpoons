#include "OgreInterface.h"
#include <iostream>
void OgreInterface::createRoot()
{
	#if _DEBUG
		mRoot = new Ogre::Root("plugins_d.cfg");
	#else
		mRoot = new Ogre::Root("plugins.cfg");
	#endif
}

bool OgreInterface::setConfiguration()
{
	/*if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
		return false;*/
	RenderSystemList l = mRoot->getAvailableRenderers();
	rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	return true;
}

void OgreInterface::createWindow()
{
	mWindow = mRoot->initialise(true, "TutorialApplication Render Window");
}

void OgreInterface::initApp()
{
	createRoot();
	setConfiguration();
	createWindow();
}
