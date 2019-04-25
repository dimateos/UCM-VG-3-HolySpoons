#include "CameraMenu.h"
#include <OgreSceneNode.h>

void CameraMenu::setUp() {
	if (isInited()) return;
	setInited();

	//cam
	camNode_ = RenderSystemInterface::getSingleton()->getCameraNode();
}

void CameraMenu::lateSetUp() {
	camNode_->setPosition(nap_vector3(cfg_["pos"]).ogre());
	camNode_->lookAt(nap_vector3(cfg_["dir"]).ogre() * ogre_scale, Ogre::Node::TS_WORLD);
}

#include "GOFactory.h"
REGISTER_TYPE(CameraMenu);