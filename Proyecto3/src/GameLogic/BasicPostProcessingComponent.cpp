#include "BasicPostProcessingComponent.h"

#include <RenderSystemInterface.h>
#include "Messages.h"

void BasicPostProcessingComponent::setUp() {
	if (isInited()) return;
	setInited();

	compositorName = this->cfg_["CompositorName"];
	RenderSystemInterface::getSingleton()->addCompositor(compositorName);

	bool enabledOnStart = true;
	if (FIND(this->cfg_, "EnabledOnStart")) enabledOnStart = this->cfg_["EnabledOnStart"];
	currentlyActive = enabledOnStart;

	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, enabledOnStart);
}

void BasicPostProcessingComponent::setPostProcessingEnabled(bool enabled)
{
	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, enabled);
	currentlyActive = enabled;
}

std::string BasicPostProcessingComponent::getCompositorName()
{
	return compositorName;
}

void BasicPostProcessingComponent::setCompositorName(std::string name)
{
	compositorName = name;
}

void BasicPostProcessingComponent::receive(Message * msg)
{
	if (msg->id_ == MessageId::STATE_CHANGED) {
		RenderSystemInterface::getSingleton()->addCompositor(compositorName);  //wtf this can fix the pause problem
		setPostProcessingEnabled(currentlyActive);
	}
}


#include "GOFactory.h"
REGISTER_TYPE(BasicPostProcessingComponent);
