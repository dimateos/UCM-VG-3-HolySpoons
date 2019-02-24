#pragma once
#include <OgreRoot.h>
#include <OgreBuildSettings.h>
//#include <OgreOverlaySystem.h>
#include <OgreFrameListener.h>
#include <SDL.h>

namespace OgreBites
{
	typedef SDL_Window NativeWindowType;

	/**
	link between a renderwindow and a platform specific window
	*/
	struct NativeWindowPair
	{
		Ogre::RenderWindow* render = nullptr;
		NativeWindowType* native = nullptr;
	};

	class OgreInterface : public Ogre::FrameListener
	{
	private:
		Ogre::Root* root;
		//Ogre::OverlaySystem* mOverlaySystem;  // Overlay system
		NativeWindowPair mWindow; // the window
		Ogre::String wName = "Proyecto 3";
		virtual void createRoot();
		virtual void setup();
		/**
		Create a new render window
		You must use SDL and not an auto-created window as SDL does not get the events otherwise.
		By default the values from ogre.cfg are used for w, h and miscParams.
		*/
		virtual NativeWindowPair createWindow(const Ogre::String& name);
		void setWindowGrab(bool _grab);
		/**
		Finds context-wide resource groups. I load paths from a config file here,
		but you can choose your resource locations however you want.
		*/
		virtual void locateResources();
		/**
		Initialize the RT Shader system.
		*/
		bool initialiseRTShaderSystem();
		/**
		Loads context-wide resource groups. I chose here to simply initialise all
		groups, but you can fully load specific ones if you wish.
		*/
		virtual void loadResources();
		void shutdown();

	public:
		OgreInterface();
		virtual ~OgreInterface();

		/**
		This function initializes the render system and resources.
		*/
		void initApp();
		void closeApp();
	};
}

