#include "PlaneComponent.h"

#include <RenderSystemInterface.h>
#include <OgreSceneNode.h>

void PlaneComponent::setUp() {

	RenderSystemInterface* rsi = RenderSystemInterface::getSingleton();

	rsi->createPlane(cfg_["name"], Ogre::Vector3(cfg_["normal"]["x"], cfg_["normal"]["y"], cfg_["normal"]["z"]),
		cfg_["w"], cfg_["h"], Ogre::Vector3(cfg_["up"]["x"], cfg_["up"]["y"], cfg_["up"]["z"])).first->
		setPosition(Ogre::Vector3(cfg_["pos"]["x"], cfg_["pos"]["y"], cfg_["pos"]["z"]));

	rsi->setMaterial(cfg_["name"], cfg_["material"]);
}

#include "GOFactory.h"
REGISTER_TYPE(PlaneComponent);