#include "RenderSystemManager.h"
#include "RenderSystemInterface.h"

int main(int argc, char *argv[])
{
	//see if there are memory leaks (in debug mode)
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	RenderSystemManager* renderManager = RenderSystemManager::getSingleton();
	RenderSystemInterface* interfaceManager = RenderSystemInterface::createSingleton(renderManager->getSceneManager());

	interfaceManager->LOGGER();
	bool exit_ = true;
	while (exit_) {

		SDL_Event evt;
		while (SDL_PollEvent(&evt)) {
			if (evt.type == SDL_QUIT) {
				exit_ = false;
			}
			renderManager->handleEvents(evt);
		}

		renderManager->renderFrame();
	}
	renderManager->shutdown();
	return 0;
}