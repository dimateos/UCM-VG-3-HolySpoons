#include "RenderSystemManager.h"
#include "RenderSystemInterface.h"

int main(int argc, char *argv[])
{
	//see if there are memory leaks (in debug mode)
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	RenderSystemManager* renderManager = RenderSystemManager::getSingleton();
	RenderSystemInterface* interfaceManager = RenderSystemInterface::createSingleton(renderManager->getSceneManager());
	
	SceneNode* n = interfaceManager->createOgreEntity("Sinbad", "Sinbad.mesh");
	n->setScale(Vector3(35, 35, 35));
	Entity* en = interfaceManager->getEntityByName("Sinbad");

	en->getAnimationState("Dance")->setEnabled(true); //una vez haces esto, hay que pasarle el tiempo de alguna manera ??
	en->getAnimationState("Dance")->setLoop(true);

	interfaceManager->setAmbientLight(ColourValue(0.5, 0, 0.5));

	//queda probar lo de los hijos y el setMaterial
	
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
		en->getAnimationState("Dance")->addTime(0.01);
	}
	renderManager->shutdown();
	return 0;
}