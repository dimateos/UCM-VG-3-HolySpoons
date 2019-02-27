#pragma once
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

using namespace Ogre;

class OgreInterface
{
private:
	Root* mRoot = nullptr;
	RenderWindow* mWindow = nullptr;
	RenderSystem* rs = nullptr;
	SceneManager* mSceneMgr = nullptr;
	Camera* mCamera = nullptr;
	SceneNode* mCamNode = nullptr;
	SceneNode* mLightNode = nullptr;
	Ogre::Light* mainLight = nullptr;
	Viewport* vp = nullptr;
	std::string projectName = "Holy Spoons";
	void createRoot();
	void setupResources();
	bool setConfiguration();
	void createWindow();
	void initializeResources();
	void createSceneManager();
	void setupScene();

public:
	OgreInterface(): mRoot(0) {};
	virtual ~OgreInterface() { delete mRoot; };
	void initApp();
};

/*
TODO:
	*Solucionar inicializar recursos
	*Object Input (no encuentra los archivos -> no los tenemos)
	*ESTA CLASE TIENE QUE SER SINGLETON!!!!
*/

