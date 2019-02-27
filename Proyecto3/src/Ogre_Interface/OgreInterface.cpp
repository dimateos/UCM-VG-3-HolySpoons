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

void OgreInterface::setupResources()
{
	Ogre::ConfigFile cf;
	#if _DEBUG
		cf.load("resources_d.cfg");
	#else
		cf.load("resources.cfg");
	#endif

	Ogre::String sec, type, arch;
	Ogre::ConfigFile::SettingsBySection_::const_iterator seci;

	//secciones
	for (seci = cf.getSettingsBySection().begin(); seci != cf.getSettingsBySection().end(); ++seci)
	{
		sec = seci->first;
		const Ogre::ConfigFile::SettingsMultiMap& settings = seci->second;
		Ogre::ConfigFile::SettingsMultiMap::const_iterator i;

		//elementos de cada seccion
		for (i = settings.begin(); i != settings.end(); ++i)
		{
			type = i->first;
			arch = FileSystemLayer::resolveBundlePath(i->second);
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}

	sec = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME;

	const Ogre::ResourceGroupManager::LocationList genLocs = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(sec);
}

bool OgreInterface::setConfiguration()
{
	RenderSystemList l = mRoot->getAvailableRenderers();
	rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	return true;
}

void OgreInterface::createWindow()
{
	mWindow = mRoot->initialise(true, projectName);
}

void OgreInterface::initializeResources()
{
	
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void OgreInterface::createSceneManager()
{
	mSceneMgr = mRoot->createSceneManager();
}

void OgreInterface::setupScene()
{
	//Camara
	mCamera = mSceneMgr->createCamera("MainCam");

	mCamera->setNearClipDistance(1);
	mCamera->setFarClipDistance(10000);
	mCamera->setAutoAspectRatio(true);

	mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mCamera);

	mCamNode->setPosition(0, 0, 1000);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	//Viewport
	vp = mWindow->addViewport(mCamera);

	vp->setBackgroundColour(ColourValue(1, 1, 1));
	mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

	//Lights
	mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));
	mainLight = mSceneMgr->createLight("MainLight");
	mainLight->setType(Ogre::Light::LT_DIRECTIONAL);
	mainLight->setDiffuseColour(0.75, 0.75, 0.75);
	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(mainLight);

	mLightNode->setDirection(Ogre::Vector3(0, 0, -1));


	//finally something to render yas (Sinbad de prueba)
	Entity* ogreEntity = mSceneMgr->createEntity("Sinbad.mesh");;
	SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(ogreEntity);
	ogreNode->setScale(Vector3(35, 35, 35));

	mRoot->startRendering();
}

void OgreInterface::initApp()
{
	createRoot();
	setupResources();
	setConfiguration();
	createWindow();
	initializeResources();
	createSceneManager();
	setupScene();
}
