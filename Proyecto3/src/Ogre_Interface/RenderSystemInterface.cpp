#include "RenderSystemInterface.h"
#include "RenderSystemManager.h"

#include <OgreResourceGroupManager.h>
#include <OgreMeshManager.h>
#include <OgreLogManager.h>
#include <OgreLog.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayContainer.h>
#include <OgreOverlay.h>
#include <OgreFontManager.h>
#include <OgreBuildSettings.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include "OgreViewport.h" //A CPP
#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreRoot.h>
#include <OgreCompositorManager.h>

#include <LogSystem.h>
#include <Transforms.h>

using namespace Ogre;

RenderSystemInterface* RenderSystemInterface::instance_ = nullptr;

RenderSystemInterface * RenderSystemInterface::getSingleton()
{
	if (instance_ == nullptr) {
		instance_ = new RenderSystemInterface();
	}

	return instance_;
}

RenderSystemInterface::RenderSystemInterface ()
{
	overlayManager = OverlayManager::getSingletonPtr();
}

RenderSystemInterface::~RenderSystemInterface()
{
	overlayManager->destroyAllOverlayElements();
}

void RenderSystemInterface::shutdownSingleton()
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

inline void RenderSystemInterface::setSceneManager(Ogre::SceneManager * s)
{
	mScnMgr = s;
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

SceneNode * RenderSystemInterface::createLight(std::string name, LightTypes type, ColourValue diffColor, ColourValue specColor, Real range)
{
	Ogre::Light* l = getSceneManager()->createLight(name);
	l->setType(Ogre::Light::LightTypes(type));
	l->setDiffuseColour(diffColor);
	l->setSpecularColour(specColor);
	setLightRange(l, range);
	SceneNode* mLightNode = getRootSceneNode()->createChildSceneNode(name);
	mLightNode->attachObject(l);

	return mLightNode;
}

void RenderSystemInterface::setLightRange(Ogre::Light * L, Ogre::Real Range)
{
	L->setAttenuation(Range, 1.0f, 4.5 / Range, 75.0f / (Range*Range));
}

OgrePair RenderSystemInterface::createPlane(std::string name, Vector3 Normal,Real w, Real h, Vector3 up)
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

Ogre::OverlayElement * RenderSystemInterface::createOverlayElement(std::string type, std::string name)
{
	OverlayElement* e = overlayManager->createOverlayElement(type, name);
	return e;
}

void RenderSystemInterface::setOverlayElementDimensions_rel(Ogre::OverlayElement* e, float w, float h)
{
	e->setDimensions(w, h);
}
void RenderSystemInterface::setOverlayElementDimensions_abs(Ogre::OverlayElement* e, float w, float h)
{
	e->setDimensions(w/ getCamera()->getViewport()->getActualWidth(), h/ getCamera()->getViewport()->getActualHeight());
}

void RenderSystemInterface::setOverlayElementPosition_rel(Ogre::OverlayElement* e, float x, float y)
{
	e->setPosition(x ,y);
}
void RenderSystemInterface::setOverlayElementPosition_abs(Ogre::OverlayElement* e, float x, float y)
{
	e->setPosition(x/getCamera()->getViewport()->getActualWidth(), y/ getCamera()->getViewport()->getActualHeight());
}

void RenderSystemInterface::setOverlayElementCenteredPosition_rel(Ogre::OverlayElement * e, float x, float y)
{
	float eW = e->getWidth() / getCamera()->getViewport()->getWidth(),
		eH = e->getHeight() / getCamera()->getViewport()->getHeight();
	e->setPosition(x - eW / 2, y - eH / 2);
}
void RenderSystemInterface::setOverlayElementCenteredPosition_abs(Ogre::OverlayElement * e, float x, float y)
{
	e->setPosition(x / getCamera()->getViewport()->getActualWidth() - e->getWidth() / 2,
		y / getCamera()->getViewport()->getActualHeight() - e->getHeight() / 2);
}

Ogre::OverlayElement * RenderSystemInterface::getOverlayElement(std::string name)
{
	return overlayManager->getOverlayElement(name);
}

void RenderSystemInterface::setOverlayElementMaterial(Ogre::OverlayElement * e, std::string matName)
{
	e->setMaterialName(matName);
}

Ogre::Quaternion RenderSystemInterface::getRotationFrom_To(Ogre::Vector3 src, Ogre::Vector3 dest)
{
	return src.getRotationTo(dest);
}

nap_quat RenderSystemInterface::getRotationFrom_To(nap_vector3 src, nap_vector3 dest)
{
	Vector3 src_ = src.ogre();
	Vector3 dest_ = dest.ogre();
	src_.normalise();
	dest_.normalise();
	Quaternion quat = src_.getRotationTo(dest_);

	nap_quat nq = {quat.w, quat.x, quat.y, quat.z};

	return nq;
}

void RenderSystemInterface::setSkyBox(std::string material, float distance)
{
	mScnMgr->setSkyBox(true, material);
}

void RenderSystemInterface::setRenderingScene(std::string scene)
{
	RenderSystemManager::getSingleton()->_setRenderingScene(scene);
	setSceneManager(RenderSystemManager::getSingleton()->getCurrentSceneManager());
	camera = getSceneManager()->getCamera("MainCam");

	currentRenderingScene = scene;
}

std::string RenderSystemInterface::getCurrentRenderingScene()
{
	return currentRenderingScene;
}

void RenderSystemInterface::addToPanel(std::string name, Ogre::OverlayElement* elemt)
{
	static_cast<OverlayContainer*>(overlayManager->getOverlayElement(name))->addChild(elemt);
}

void RenderSystemInterface::addCompositor(std::string name)
{
	CompositorManager::getSingleton().addCompositor(getViewport(), name);
}

void RenderSystemInterface::setCompositorEnabled(std::string name, bool en)
{
	CompositorManager::getSingleton().setCompositorEnabled(getViewport(), name, en);
}
