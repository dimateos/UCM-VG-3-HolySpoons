#ifndef RENDERSYSTEMINTERFACE_H_
#define RENDERSYSTEMINTERFACE_H_

#include <OgreBuildSettings.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
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

	/*
	 *Devuelve el RootNode de la escena
	 */
	inline SceneNode* getRootSceneNode() { return mScnMgr->getRootSceneNode(); };
	/*
	 *Devuelve el Scene Manager
	 */
	inline SceneManager* getSceneManager() { return mScnMgr; };
	/*
	 *Busca la entidad por nombre y la devuelve
	 */
	inline Entity* getEntityByName(String name) { return mScnMgr->getEntity(name);};
	/*
	 *Crea una entidad segun el nombre
	 */
	SceneNode* createOgreEntity(String name);
	/*
	 *Añade un hijo al nodo que le pases
	 */
	SceneNode* addChild(SceneNode* father, String name);
	
	
};

#endif /*RENDERSYSTEMINTERFACE_H_*/

