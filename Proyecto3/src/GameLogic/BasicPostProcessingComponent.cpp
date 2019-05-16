#include "BasicPostProcessingComponent.h"

#include <RenderSystemInterface.h>
#include "Messages.h"

void BasicPostProcessingComponent::setUp() {
	if (isInited()) return;
	setInited();

	string s = this->cfg_["CompositorName"];
	compositorName = s;
	RenderSystemInterface::getSingleton()->addCompositor(compositorName);

	bool enabledOnStart = true;
	if (FIND(this->cfg_, "EnabledOnStart")) enabledOnStart = this->cfg_["EnabledOnStart"];
	currentlyActive = enabledOnStart;

	if (FIND(this->cfg_, "Active_Message")) active_message_ = this->cfg_["Active_Message"];
	if (FIND(this->cfg_, "Inactive_Message")) inactive_message_ = this->cfg_["Inactive_Message"];

	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, true);
	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, enabledOnStart);
}

void BasicPostProcessingComponent::setPostProcessingEnabled(bool enabled)
{
	currentlyActive = enabled;
	RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, enabled);
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
	if (msg->id_ == MessageId::STATE_IN) {
		RenderSystemInterface::getSingleton()->addCompositor(compositorName); //changing ogre scene resets the compositor
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
	else if (msg->id_ == (MessageId)active_message_) {
		currentlyActive = true;
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
	else if (msg->id_ == (MessageId)inactive_message_) {
		currentlyActive = false;
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(BasicPostProcessingComponent);
