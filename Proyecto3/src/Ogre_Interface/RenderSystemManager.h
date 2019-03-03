#ifndef RENDERSYSTEMMANAGER_H_
#define RENDERSYSTEMMANAGER_H_

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
//RenderSystemManager

class RenderSystemManager : public FrameListener
{
private:
	static RenderSystemManager* instance_; //singleton pattern

	//Window and scene management
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

	void initApp();
	void createRoot();
	void setupResources();
	bool setConfiguration();
	void createWindow();
	void initializeResources();
	void createSceneManager();
	void setupScene();

	RenderSystemManager() : mRoot(0) { initApp(); }; //private constructor
	virtual ~RenderSystemManager() {};

public:
	/*
	First time called --> creates and returns pointer to RenderSystemManager, initializes the app, starts rendering
	2 or + times called --> returns pointer to RenderSystemManager 
	*/
	static RenderSystemManager* getSingleton();

	/*
	Shutdowns the entire app
	*/
	void shutdown();

	/*
	 *process all window events since last call
	 */
	void pollEvents();

	/*
	Called when the frame starts rendering
	*/
	virtual bool frameStarted(const Ogre::FrameEvent& evt) { pollEvents(); return true; };

	/*
	Called when the frame ends rendering
	*/
	virtual bool frameEnded(const Ogre::FrameEvent& evt) { return true; };

	//Virtual methods, dont really need to be changed
	virtual void windowMoved(Ogre::RenderWindow* rw) {};
	virtual void windowResized(Ogre::RenderWindow* rw) {};
	virtual bool windowClosing(Ogre::RenderWindow* rw) { return true; };
	virtual void windowClosed(Ogre::RenderWindow* rw) {};
	virtual void windowFocusChange(Ogre::RenderWindow* rw) {};

	inline const SceneManager* getSceneManager() const { return mSceneMgr; };
};

#endif

/*
*TODO:
	*Profiler
	Interfaz de verdad --> otra clase con acceso al scene manager
*/

