#include "TimedPostProcessingComponent.h"

#include <RenderSystemInterface.h>
#include "Messages.h"

void TimedPostProcessingComponent::setUp() {
	if (isInited()) return;
	setInited();

	std::string s = this->cfg_["CompositorName"];
	compositorName = s;

	RenderSystemInterface::getSingleton()->addCompositor(compositorName);
	timer.setDuration(cfg_["ActiveTime"]);
	timer.stop();
}

void TimedPostProcessingComponent::update(GameObject* ent, double time)
{
	if (timer.update(time)) {
		currentlyActive = false;
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
}

void TimedPostProcessingComponent::receive(Message * msg)
{
	if (msg->id_ == HP_DAMAGE) {
		if (!currentlyActive) {
			currentlyActive = true;
			RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
			timer.start();
		}
	}
	else if (msg->id_ == MessageId::STATE_CHANGED) {
		RenderSystemInterface::getSingleton()->addCompositor(compositorName);  //wtf this can fix the pause problem
		RenderSystemInterface::getSingleton()->setCompositorEnabled(compositorName, currentlyActive);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(TimedPostProcessingComponent);
