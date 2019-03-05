#include "RenderSystemInterface.h"

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

SceneNode* RenderSystemInterface::createOgreEntity(String name)
{
	SceneNode* ogreNode = nullptr;
	Entity* entity = mScnMgr->createEntity(name);
	ogreNode = mScnMgr->getRootSceneNode()->createChildSceneNode();
	ogreNode->attachObject(entity);
	return ogreNode;
}

SceneNode * RenderSystemInterface::addChild(SceneNode * father, String name)
{
	SceneNode* child = createOgreEntity(name);
	father->addChild(child);
	return child;
}
