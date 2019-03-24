#include <OgreResourceGroupManager.h>
#include "RenderSystemInterface.h"
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <LogSystem.h>
#include <OgreLog.h>

RenderSystemInterface* RenderSystemInterface::instance_ = nullptr;

RenderSystemInterface* RenderSystemInterface::createSingleton(SceneManager* mScnMgr)
{
	if (instance_ == nullptr) {
		instance_ = new RenderSystemInterface(mScnMgr);
	}

	return instance_;
}

RenderSystemInterface * RenderSystemInterface::getSingleton()
{
	return instance_;
}

void RenderSystemInterface::closeInterface()
{
	delete instance_;
}

OgrePair RenderSystemInterface::createOgreEntity(String name, String meshName)
{
	SceneNode* ogreNode = nullptr;
	Entity* entity = meshName == "" ? mScnMgr->createEntity(name) : mScnMgr->createEntity(name,meshName);
	ogreNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(entity);
	std::pair<SceneNode*, Entity*> p (ogreNode, entity);
	return p;
}


SceneNode * RenderSystemInterface::createEmpty(String name)
{
	return getRootSceneNode()->createChildSceneNode(name);
}

SceneNode * RenderSystemInterface::createLight(String name, Light::LightTypes type, ColourValue color)
{
	Ogre::Light* l = getSceneManager()->createLight(name);
	l->setType(type);
	l->setDiffuseColour(color);

	SceneNode* mLightNode = getRootSceneNode()->createChildSceneNode(name);
	mLightNode->attachObject(l);

	return mLightNode;
}

OgrePair RenderSystemInterface::createPlane(String name, Vector3 Normal,Real w, Real h, Vector3 up)
{
	try{
		MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Normal, 0), w, h, 100, 80, true, 1, 1.0, 1.0, up); //Crea una mesh
	}
	catch (Exception e) {
		LogSystem::Log("VECTORES INVALIDOS EN EL PLANO   "+ (string)e.what(), LogSystem::REND);
		//LogManager::getSingleton().getLog("Ogre.log")->logMessage("SO GILIPOLLAS LOS VECTORES SON IVALIDOS");
		MeshManager::getSingleton().remove(name);
		MeshManager::getSingleton().createPlane(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Plane(Vector3::UNIT_Z, 0), w, h, 100, 80, true, 1, 1.0, 1.0, Vector3::UNIT_Y); //Crea una mesh
	}


	OgrePair p = createOgreEntity(name, name); //Crea la entidad con la mesh
	return p;
}

void RenderSystemInterface::setAmbientLight(ColourValue color )
{
	getSceneManager()->setAmbientLight(color);
}

SceneNode * RenderSystemInterface::addChild(SceneNode * father, String name, String meshName)
{
	SceneNode* child = createOgreEntity(name,meshName).first;
	child->getParentSceneNode()->removeChild(child); //needed
	father->addChild(child);
	return child;
}

void RenderSystemInterface::addChild(SceneNode * father, SceneNode * child)
{
	child->getParentSceneNode()->removeChild(child); //needed
	father->addChild(child);
}

void RenderSystemInterface::setMaterial(String entity, String material)
{
	getSceneManager()->getEntity(entity)->setMaterialName(material);
}

void RenderSystemInterface::setMaterial(Entity * entity, String material)
{
	entity->setMaterialName(material);
}

SceneNode * RenderSystemInterface::getNode(String name)
{
	try
	{
		SceneNode* node = static_cast<SceneNode*>(getRootSceneNode()->getChild(name));
		return node;
	}
	catch (const std::exception e)
	{
		LogSystem::Log("ERROR AL ACCEDER AL NODO " + name + "   " + (string)e.what(), LogSystem::REND);
		return nullptr;
	}
}
