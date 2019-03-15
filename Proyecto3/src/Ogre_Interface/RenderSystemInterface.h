#ifndef RENDERSYSTEMINTERFACE_H_
#define RENDERSYSTEMINTERFACE_H_

#include <OgreBuildSettings.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "OgreViewport.h"
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreRoot.h>

using namespace Ogre;
class RenderSystemInterface
{
private:
	SceneManager * mScnMgr = nullptr;
	Camera* camera = nullptr;

	static RenderSystemInterface* instance_; //singleton pattern
	RenderSystemInterface(SceneManager * mScnMgr) : mScnMgr(mScnMgr) { camera = getSceneManager()->getCamera("MainCam"); };
	virtual ~RenderSystemInterface() {};

public:
	static RenderSystemInterface* createSingleton(SceneManager * mScnMgr);
	static RenderSystemInterface* getSingleton();
	void closeInterface();
	//Resto de interfaz shaders (?), animacion, camara, viewport...

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
	 *Crea una entidad + nodo segun el nombre
	 */
	std::pair<SceneNode*, Entity*> createOgreEntity(String name, String meshName = "");
	/*
	 *Crea un nodo vacio
	 */
	SceneNode* createEmpty(String name);
	/*
	 *Crea una luz del tipo elegido. La direccion se setea con el nodo (node->setDirection(Ogre::Vector3(x, y, z));)
	 */
	SceneNode* createLight(String name, Light::LightTypes type, ColourValue color);
	/*
	 *Crea un nodo vacio
	 * UP NO PUEDE SER IGUAL A NORMAL
	 */
	std::pair<SceneNode*, Entity*> createPlane(String name, Vector3 Normal, Real w, Real h,Vector3 up);
	/*
	 *set de la luz ambiental
	 */
	void setAmbientLight(ColourValue color);
	/*
	 *A�ade un hijo al nodo que le pases
	 */
	SceneNode* addChild(SceneNode* father, String name,String meshName);
	/*
	 *A�ade un hijo (ya creado) al nodo que le pases
	 */
	void addChild(SceneNode* father, SceneNode* child);
	/*
	 *Set del material a una entidad
	 */
	void setMaterial(String entity, String material);
	/*
	 *Set del material con acceso a la entidad
	 */
	void setMaterial(Entity* entity, String material);
	/*
	 *Get node by nombre
	 */
	SceneNode* getNode(String name);
	/*
	 *Get camera (modify nearClipDistance, FarClip, AspectRatio...)
	 */
	inline Camera* getCamera() { return camera; };
	/*
	 *Get cameraNode (modify position, lookAt...)
	 */
	inline SceneNode* getCameraNode() { return camera->getParentSceneNode(); };
};

#endif /*RENDERSYSTEMINTERFACE_H_*/

