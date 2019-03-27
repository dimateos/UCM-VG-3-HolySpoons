#ifndef RENDERSYSTEMMANAGER_H_
#define RENDERSYSTEMMANAGER_H_

#include <OgreFrameListener.h>
#include <string>

//#include <LogSystem.h>
typedef struct SDL_Window SDL_Window;
typedef union SDL_Event;

namespace Ogre {
	class OverlaySystem;
	class Root;
	class RenderWindow;
	class SceneManager;
	class Camera;
	class RenderSystem;
	class SceneNode;
	class Light;
	class Viewport;
};

class RenderSystemManager : public Ogre::FrameListener
{
private:
	static RenderSystemManager* instance_; //singleton pattern
	//Window and scene management
	Ogre::Root* mRoot = nullptr;
	Ogre::RenderWindow* mWindow = nullptr;
	SDL_Window* SDL_win = nullptr;
	Ogre::RenderSystem* rs = nullptr;
	Ogre::SceneManager* mSceneMgr = nullptr;
	Ogre::Camera* mCamera = nullptr;
	Ogre::SceneNode* mCamNode = nullptr;
	Ogre::SceneNode* mLightNode = nullptr;
	Ogre::Light* mainLight = nullptr;
	Ogre::Viewport* vp = nullptr;
	Ogre::OverlaySystem * overlaySystem = nullptr;

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

	void renderFrame();

	/*
	*process all window events since last call
	*/
	bool handleEvents(const SDL_Event evt);

	/*
	Shutdowns the entire app
	*/
	void shutdown();

	/*
	Called when the frame starts rendering
	*/
	virtual bool frameStarted(const Ogre::FrameEvent& evt) { return true; };

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

	inline Ogre::SceneManager* getSceneManager() const { return mSceneMgr; };

	inline  Ogre::Root* getRoot()  { return mRoot; };
};

#endif

