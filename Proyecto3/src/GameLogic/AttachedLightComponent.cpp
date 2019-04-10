#include "AttachedLightComponent.h"
#include "RenderComponent.h"
#include <OgreColourValue.h>

void AttachedLightComponent::setLight() {
	std::string name = cfg_["name"];
	lightNode = RenderSystemInterface::getSingleton()->createLight(name + id().sn_string(),
		cfg_["type"], Ogre::ColourValue(cfg_["diffColor"]["r"], cfg_["diffColor"]["g"],
			cfg_["diffColor"]["b"]), Ogre::ColourValue(cfg_["specColor"]["r"], cfg_["specColor"]["g"],
				cfg_["diffColor"]["b"]), cfg_["range"]);

	Ogre::SceneNode* father = static_cast<RenderComponent*>(owner_->
		getComponent("basic_ren"))->getSceneNode();

	RenderSystemInterface::getSingleton()->addChild(father, lightNode);
}

void AttachedLightComponent::setUp() {

}

void AttachedLightComponent::lateSetUp() {
	if (isInited()) return;
	setInited();

	setLight();
}
