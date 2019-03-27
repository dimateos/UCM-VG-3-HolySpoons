#include <OgreResourceGroupManager.h>
#include "RenderSystemInterface.h"
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <LogSystem.h>
#include <OgreLog.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayManager.h>
//#include <OgreOverlayContainer.h>
//#include <OgreOverlay.h>
#include <OgreFontManager.h>
#include <OgreBuildSettings.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "OgreViewport.h" //A CPP
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreRoot.h>

using namespace Ogre;

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

RenderSystemInterface::RenderSystemInterface (SceneManager * mScnMgr): mScnMgr(mScnMgr)
{
	camera = getSceneManager()->getCamera("MainCam");
	overlayManager = OverlayManager::getSingletonPtr();
}

RenderSystemInterface::~RenderSystemInterface()
{
}

void RenderSystemInterface::closeInterface()
{
	delete instance_;
}

inline Ogre::SceneNode* RenderSystemInterface::getRootSceneNode()
{
	return mScnMgr->getRootSceneNode();
}

inline Ogre::SceneManager * RenderSystemInterface::getSceneManager()
{
	return mScnMgr;
}

inline Ogre::Entity * RenderSystemInterface::getEntityByName(std::string name)
{
	return mScnMgr->getEntity(name);
}

OgrePair  RenderSystemInterface::createOgreEntity(std::string name, std::string meshName)
{
	SceneNode* ogreNode = nullptr;
	Entity* entity = meshName == "" ? mScnMgr->createEntity(name) : mScnMgr->createEntity(name,meshName);
	ogreNode = mScnMgr->getRootSceneNode()->createChildSceneNode(name);
	ogreNode->attachObject(entity);
	OgrePair p (ogreNode, entity);
	return p;
}

SceneNode * RenderSystemInterface::createEmpty(std::string name)
{
	return getRootSceneNode()->createChildSceneNode(name);
}

SceneNode * RenderSystemInterface::createLight(std::string name, LightTypes type, ColourValue color)
{
	Ogre::Light* l = getSceneManager()->createLight(name);
	l->setType(Ogre::Light::LightTypes(type));
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

SceneNode * RenderSystemInterface::addChild(SceneNode * father, std::string name, std::string meshName)
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

void RenderSystemInterface::setMaterial(std::string entity, std::string material)
{
	getSceneManager()->getEntity(entity)->setMaterialName(material);
}

void RenderSystemInterface::setMaterial(Entity * entity, std::string material)
{
	entity->setMaterialName(material);
}

SceneNode * RenderSystemInterface::getNode(std::string name)
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

inline Ogre::Camera * RenderSystemInterface::getCamera()
{
	return camera;
}

Ogre::SceneNode * RenderSystemInterface::getCameraNode()
{
	return camera->getParentSceneNode();
}

inline Ogre::Viewport * RenderSystemInterface::getViewport()
{
	return camera->getViewport();
}

TextAreaOverlayElement * RenderSystemInterface::createText(std::string nodeName, std::string text, int x, int y, std::string fontName)
{
	TextAreaOverlayElement* textElement = static_cast<TextAreaOverlayElement*>(overlayManager->createOverlayElement("TextArea", nodeName));
	textElement->setMetricsMode(Ogre::GMM_PIXELS); //Maybe RELATIVE ????

	textElement->setCaption(text);
	textElement->setDimensions(100, 100);
	textElement->setFontName(fontName);

	textElement->setPosition(x, y);

	//panel->addChild(textElement);
	//le tienes que pasar un padre o el nombre de un padre maybe?

	return textElement;
}

void RenderSystemInterface::setText(TextAreaOverlayElement * element, std::string szString)
{
	element->setCaption(szString);
}

void RenderSystemInterface::setTextPosition(TextAreaOverlayElement * element, float x, float y)
{
	element->setPosition(x, y);
}

void RenderSystemInterface::setTextSize(TextAreaOverlayElement * element, float size)
{
	element->setCharHeight(size);
}

void RenderSystemInterface::setTextCenteredPosition(TextAreaOverlayElement * element, float x, float y)
{
	//not workong
	setTextPosition(element, x, y);
	element->setAlignment(TextAreaOverlayElement::Center);
	element->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_CENTER);
	element->setVerticalAlignment(Ogre::GuiVerticalAlignment::GVA_CENTER);
	element->_update();
}

void RenderSystemInterface::setTextColour(TextAreaOverlayElement * element, float R, float G, float B, float I)
{
	element->setColour(Ogre::ColourValue(R, G, B, I));
}

void RenderSystemInterface::setTextColourTop(TextAreaOverlayElement * element, float R, float G, float B, float I)
{
	element->setColourTop(Ogre::ColourValue(R, G, B, I));
}

void RenderSystemInterface::setTextColourBot(TextAreaOverlayElement * element, float R, float G, float B, float I)
{
	element->setColourBottom(Ogre::ColourValue(R, G, B, I));
}
