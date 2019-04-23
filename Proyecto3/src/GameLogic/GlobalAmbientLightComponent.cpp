#include "GlobalAmbientLightComponent.h"
#include "RenderComponent.h"
#include <RenderSystemInterface.h>
#include <OgreColourValue.h>

void GlobalAmbientLightComponent::setLight() {
	_ambientLight = { this->cfg_["r"], this->cfg_["g"], this->cfg_["b"] };
	RenderSystemInterface::getSingleton()->setAmbientLight(Ogre::ColourValue(_ambientLight.x_, _ambientLight.y_, _ambientLight.z_));
}

void GlobalAmbientLightComponent::setUp() {
	if (isInited()) return;
	setInited();
}

void GlobalAmbientLightComponent::lateSetUp() {

	setLight();
}

void GlobalAmbientLightComponent::setAmbientLight(nap_vector3 nLight)
{
	_ambientLight = nLight;
	RenderSystemInterface::getSingleton()->setAmbientLight(Ogre::ColourValue(_ambientLight.x_, _ambientLight.y_, _ambientLight.z_));
}

const nap_vector3 GlobalAmbientLightComponent::getAmbientLight() const
{
	return _ambientLight;
}

#include "GOFactory.h"
REGISTER_TYPE(GlobalAmbientLightComponent);
