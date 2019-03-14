#include "RenderSystemInterface.h"
#include <OgreLog.h>
#include <OgreLogManager.h>

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

SceneNode* RenderSystemInterface::createOgreEntity(String name, String meshName)
{
	SceneNode* ogreNode = nullptr;
	Entity* entity = meshName == "" ? mScnMgr->createEntity(name) : mScnMgr->createEntity(name,meshName);
	ogreNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(entity);
	return ogreNode;
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

void RenderSystemInterface::setAmbientLight(ColourValue color )
{
	getSceneManager()->setAmbientLight(color);
}

SceneNode * RenderSystemInterface::addChild(SceneNode * father, String name, String meshName)
{
	SceneNode* child = createOgreEntity(name,meshName);
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
		LogManager::getSingleton().getLog("Ogre.log")->logMessage(e.what());
		return nullptr;
	}
}

void RenderSystemInterface::LOGGER()
{
	LogManager::getSingleton().getLog("Ogre.log")->logMessage("PACO ESTAMOS LOGGEANDO");
}
