#ifndef RENDERSYSTEMINTERFACE_H_
#define RENDERSYSTEMINTERFACE_H_

#include <OgreBuildSettings.h>
#include <OgreRoot.h>

using namespace Ogre;
class RenderSystemInterface
{
private:
	SceneManager * mScnMgr = nullptr;
	static RenderSystemInterface* instance_; //singleton pattern
	RenderSystemInterface(SceneManager * mScnMgr): mScnMgr(mScnMgr){};
	virtual ~RenderSystemInterface() {};

public:
	static RenderSystemInterface* createSingleton(SceneManager * mScnMgr);
	static RenderSystemInterface* getSingleton();
	void closeInterface();
	//Resto de interfaz, añadir material, añadir como nodo, shaders (?), añadir como hijo, buscar nodo, camara, luces, viewport...
};

#endif /*RENDERSYSTEMINTERFACE_H_*/

