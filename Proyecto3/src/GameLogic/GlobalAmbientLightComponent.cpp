#include "GlobalAmbientLightComponent.h"
#include "RenderComponent.h"
#include <RenderSystemInterface.h>
#include <OgreColourValue.h>

void GlobalAmbientLightComponent::setLight() {
	in_ambientLight = { this->cfg_["r"], this->cfg_["g"], this->cfg_["b"] };
	RenderSystemInterface::getSingleton()->setAmbientLight(Ogre::ColourValue(in_ambientLight.x_, in_ambientLight.y_, in_ambientLight.z_));
}

void GlobalAmbientLightComponent::setUp() {
	if (isInited()) return;
	setInited();
}

void GlobalAmbientLightComponent::lateSetUp() {

	setLight();
}

#include "GOFactory.h"
REGISTER_TYPE(GlobalAmbientLightComponent);
