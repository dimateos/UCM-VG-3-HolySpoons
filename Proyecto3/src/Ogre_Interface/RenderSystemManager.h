#ifndef RENDERSYSTEMMANAGER_H_
#define RENDERSYSTEMMANAGER_H_

#include <OgreFrameListener.h>
#include <string>
#include <map>

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

class RenderSystemInterface;

class RenderSystemManager : public Ogre::FrameListener
{
private:
	static RenderSystemManager* instance_; //singleton pattern
	//Window and scene management
	Ogre::Root* mRoot = nullptr;
	Ogre::RenderWindow* mWindow = nullptr;
	SDL_Window* SDL_win = nullptr;
	Ogre::RenderSystem* rs = nullptr;
	std::map<Ogre::String, Ogre::SceneManager*> scenes;
	Ogre::SceneManager* currentSceneManager = nullptr;
	Ogre::OverlaySystem * overlaySystem = nullptr;

	const std::string projectName = "Holy Spoons";

	void initApp();
	void createRoot();
	void setupResources();
	bool setConfiguration();
	void createWindow();
	void initializeResources();
	Ogre::SceneManager* createSceneManager();

	RenderSystemManager() : mRoot(0) { initApp(); }; //private constructor
	virtual ~RenderSystemManager() {};
protected:
	friend class RenderSystemInterface;
	/*
	 *Use RenderSystemInterface::setRenderingScene(string scene) instead
	 */
	void _setRenderingScene(Ogre::String scene);

public:
	/*
	First time called --> creates and returns pointer to RenderSystemManager, initializes the app, starts rendering
	2 or + times called --> returns pointer to RenderSystemManager
	*/
	static RenderSystemManager* getSingleton();

	/*
	 *creates a scene manager, camera, lights... all neccesary elements for a scene. Seek for the name "sceneName"
	 */
	void setupScene(Ogre::String sceneName);

	void renderFrame();

	/*
	*process all window events since last call
	*/
	bool handleEvents(const SDL_Event evt);

	/*
	Shutdowns the entire app
	*/
	void shutdownSingleton();

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

	inline  Ogre::Root* getRoot()  { return mRoot; };



	Ogre::SceneManager* getCurrentSceneManager();
};

#endif

