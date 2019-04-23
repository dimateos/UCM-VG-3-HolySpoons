#include "SkyBoxComponent.h"
#include "RenderComponent.h"
#include <RenderSystemInterface.h>


void SkyBoxComponent::updateSkyBox()
{
	RenderSystemInterface::getSingleton()->setSkyBox(_skyBox);
}

void SkyBoxComponent::setUp() {
	if (isInited()) return;
	setInited();

	_skyBox = this->cfg_["skybox"];
	updateSkyBox();
}

void SkyBoxComponent::setSkyBox(std::string nSkyBox)
{
	_skyBox = nSkyBox;
	updateSkyBox();
}

const std::string SkyBoxComponent::getSkyBox() const
{
	return _skyBox;
}

#include "GOFactory.h"
REGISTER_TYPE(SkyBoxComponent);
