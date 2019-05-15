#include "HitPostProcessingComponent.h"

#include <RenderSystemInterface.h>
#include "Messages.h"

void HitPostProcessingComponent::setUp() {
	if (isInited()) return;
	setInited();

	std::string s = this->cfg_["CompositorName"];
	compositorName = s;

	RenderSystemInterface::getSingleton()->addCompositor(compositorName);
}

void HitPostProcessingComponent::setEnableComp(bool b) {
	currentlyActive = b;
	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
}

void HitPostProcessingComponent::receive(Message * msg)
{
	if (msg->id_ == HP_DAMAGE) {
		if (!currentlyActive) setEnableComp(true);
	}
	else if (msg->id_ == MessageId::END_INV) {
		if (currentlyActive) setEnableComp(false);
	}
	else if (msg->id_ == MessageId::STATE_IN) {
		RenderSystemInterface::getSingleton()->addCompositor(compositorName);  //changing ogre scene resets the compositor
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(HitPostProcessingComponent);
