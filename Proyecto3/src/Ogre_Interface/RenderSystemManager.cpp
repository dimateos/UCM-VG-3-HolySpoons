#include <SDL_video.h>
#include <SDL_syswm.h>
#include "RenderSystemManager.h"
#include <OgreOverlaySystem.h>
#include <OgreFileSystemLayer.h>
#include <OgreTextureManager.h>
#include <OgreBuildSettings.h>
#include <OgreSceneManager.h>
#include <OgreRenderSystem.h>
#include "OgreRenderWindow.h"
#include <OgreConfigFile.h>
#include <OgreSceneNode.h>
#include "OgreViewport.h"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreRoot.h>
#include <SDL.h>

using namespace Ogre;

RenderSystemManager* RenderSystemManager::instance_ = nullptr;

void RenderSystemManager::createRoot()
{
	#if _DEBUG
		mRoot = new Ogre::Root("plugins_d.cfg");
	#else
		mRoot = new Ogre::Root("plugins.cfg");
	#endif

	overlaySystem = new Ogre::OverlaySystem();
}

void RenderSystemManager::setupResources()
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

//Window size, full screen... --> read it from json
bool RenderSystemManager::setConfiguration()
{
	RenderSystemList l = mRoot->getAvailableRenderers();
	rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

	mRoot->setRenderSystem(rs);
	rs->setConfigOption("Full Screen", "No");
	rs->setConfigOption("Video Mode", "800 x 600 @ 32-bit colour");

	return true;
}

//SDL_Window stuff
void RenderSystemManager::createWindow()
{
	uint32_t w, h;
	Ogre::NameValuePairList miscParams;

	Ogre::ConfigOptionMap ropts = mRoot->getRenderSystem()->getConfigOptions();

	std::istringstream mode(ropts["Video Mode"].currentValue);
	Ogre::String token;
	mode >> w; // width
	mode >> token; // 'x' as seperator between width and height
	mode >> h; // height

	miscParams["FSAA"] = ropts["FSAA"].currentValue;
	miscParams["vsync"] = ropts["VSync"].currentValue;
	miscParams["gamma"] = ropts["sRGB Gamma Conversion"].currentValue;

	if (!SDL_WasInit(SDL_INIT_VIDEO)) SDL_InitSubSystem(SDL_INIT_VIDEO);


	Uint32 flags = SDL_WINDOW_RESIZABLE;

	if (ropts["Full Screen"].currentValue == "Yes")  flags = SDL_WINDOW_FULLSCREEN;

	SDL_win = SDL_CreateWindow(projectName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWindowWMInfo(SDL_win, &wmInfo);

	miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.win.window));

	mWindow = mRoot->createRenderWindow(projectName, w, h, false, &miscParams);
}

void RenderSystemManager::initializeResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

Ogre::SceneManager* RenderSystemManager::createSceneManager()
{
	SceneManager* s = nullptr;
	s = mRoot->createSceneManager();
	s->addRenderQueueListener(overlaySystem);
	
	return s;
}

RenderSystemManager::~RenderSystemManager()
{
	//mShaderGenerator->removeSceneManager(mSM);
	//mSM->removeRenderQueueListener(mOverlaySystem);
	//mSceneMgr->removeRenderQueueListener(overlaySystem);

	for (auto it : scenes) {
		it.second->removeRenderQueueListener(overlaySystem);
		mRoot->destroySceneManager(it.second);
	}

	if (mWindow != nullptr)
	{
		mRoot->destroyRenderTarget(mWindow);
		mWindow = nullptr;
	}

	if (SDL_win != nullptr)
	{
		SDL_DestroyWindow(SDL_win);
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		SDL_win = nullptr;
	}

	delete overlaySystem;
	overlaySystem = nullptr;

	delete mRoot;
	mRoot = nullptr;
}

void RenderSystemManager::setupScene(Ogre::String sceneName)
{
	SceneManager* s = createSceneManager();
	scenes.insert({ sceneName, s });
	//Camera
	Camera* mCamera = nullptr;
	mCamera = s->createCamera("MainCam");

	//values from global cfg class at some future momment
	mCamera->setFarClipDistance(Real(100000));
	mCamera->setAutoAspectRatio(true);

	SceneNode* mCamNode = nullptr;
	mCamNode = s->getRootSceneNode()->createChildSceneNode("nCam");
	mCamNode->attachObject(mCamera);

	mCamNode->setPosition(0, 0, 1);
	mCamNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);

	//FOV
	mCamera->setNearClipDistance(0.5);
	mCamera->setFOVy(Radian(Degree(60))); //in theory overrides near clip

	//Lights
	s->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	s->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);

	/*mainLight = mSceneMgr->createLight("MainLight");
	mainLight->setType(Light::LT_POINT);
	mainLight->setDiffuseColour(1.0, 1.0, 1.0);
	mainLight->setSpecularColour(1.0, 1.0, 1.0);
	Ogre::Real range = 8000;
	mainLight->setAttenuation(range, 1.0, 4.5/range, 75.0/(range*range));
	mLightNode = mCamNode->createChildSceneNode("nLuz");
	mLightNode->attachObject(mainLight);

	//mLightNode->setDirection(Ogre::Vector3(0, 0, -1));*/
}

RenderSystemManager* RenderSystemManager::getSingleton()
{
	if (instance_ == nullptr) {
		instance_ = new RenderSystemManager();
	}

	return instance_;
}

void RenderSystemManager::renderFrame()
{
	mRoot->renderOneFrame();
}

void RenderSystemManager::shutdownSingleton()
{
	delete instance_;
}

bool RenderSystemManager::frameStarted(const Ogre::FrameEvent & evt)
{
	return true;
}

void RenderSystemManager::_setRenderingScene(Ogre::String scene)
{
	mWindow->removeAllViewports();

	SceneManager* sm = scenes.find(scene)->second;

	Viewport* v = mWindow->addViewport(sm->getCamera("MainCam"));

	sm->getCamera("MainCam")->setAspectRatio(Real(v->getActualWidth()) / Real(v->getActualHeight())); //in case this has changed

	currentSceneManager = sm;
}

Ogre::SceneManager * RenderSystemManager::getCurrentSceneManager()
{
	return currentSceneManager;
}

//void RenderSystemManager::prueba()
//{
	/*if (!kk) {
		Camera* c = sSceneMgr->getCamera("SecondCam");
		mWindow->addViewport(c);
		kk = true;
	}
	else {
		mWindow->addViewport(mCamera);
		kk = false;
	}*/
//}

//initializes the app and starts rendering
void RenderSystemManager::initApp()
{
	createRoot();
	setupResources();
	setConfiguration();
	mRoot->initialise(false);
	createWindow();
	initializeResources();
	mRoot->addFrameListener(this);
	SDL_ShowCursor(SDL_DISABLE); //Standard cursor must be turned off

	//mRoot->startRendering(); // blocks the flow
	//mRoot->renderOneFrame(); // we'll have to use this
}

bool RenderSystemManager::handleEvents(const SDL_Event evt) {
	bool handled = false;

		switch (evt.type) {
		case SDL_WINDOWEVENT:
			if (evt.window.windowID == SDL_GetWindowID(SDL_win)) {
				if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
					Ogre::RenderWindow* win = mWindow;
					win->windowMovedOrResized();
					windowResized(win);
					//handled = true; //tmp for crosshair repositioning
				}
			}
			break;

		default:
			//_fireInputEvent(convert(event));
			break;
		}

		return handled;
}