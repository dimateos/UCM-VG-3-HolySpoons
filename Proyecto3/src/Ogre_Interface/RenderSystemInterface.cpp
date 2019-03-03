#include "RenderSystemInterface.h"

RenderSystemInterface* RenderSystemInterface::instance_ = nullptr;

RenderSystemInterface* RenderSystemInterface::createSingleton(SceneManager* mScnMgr)
{
	if (instance_ == nullptr) {
		instance_ = new RenderSystemInterface(mScnMgr);
	}

	return instance_;
}

RenderSystemInterface * RenderSystemInterface::getSingleton()
{
	return instance_;
}

void RenderSystemInterface::closeInterface()
{
	delete instance_;
}
