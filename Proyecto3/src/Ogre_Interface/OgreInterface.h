#pragma once
#include <OgreFileSystemLayer.h>
#include <OgreTextureManager.h>
#include <OgreFrameListener.h>
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

class OgreInterface : public FrameListener
{
private:
	Root* mRoot = nullptr;
	RenderWindow* mWindow = nullptr;
	SDL_Window* SDL_win = nullptr;
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
	/*
	 *process all window events since last call
	 */
	void pollEvents();

	// callback interface copied from various listeners to be used by ApplicationContext
	virtual bool frameStarted(const Ogre::FrameEvent& evt) { pollEvents(); return true; }
	virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; }
	/*
	*HOLA BUENAS AMIGOS
	*/
	virtual void windowMoved(Ogre::RenderWindow* rw) {}
	virtual void windowResized(Ogre::RenderWindow* rw) {}
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; }
	virtual void windowClosed(Ogre::RenderWindow* rw) {}
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {}

};

/*
*TODO:
	*Profiler
	*ESTA CLASE TIENE QUE SER SINGLETON!!!!
	Ordenar / comentar
 
*/

