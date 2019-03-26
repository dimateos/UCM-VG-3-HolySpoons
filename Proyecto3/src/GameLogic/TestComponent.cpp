#include "TestComponent.h"
#include "LogSystem.h"

#include <RenderSystemInterface.h>
#include <OgreSceneNode.h>
#include <OgreNode.h>

void TestComponent::setUp(nap_json const & cfg) {
	RenderSystemInterface* rsi = RenderSystemInterface::getSingleton();

	//auto light = renderInterface->createLight("light", Ogre::Light::LT_POINT, ColourValue(0.75f, 0.2f, 0.2f));
	//light->setPosition(-1.0f, 0.0f, 0.0f);

	//text
	//OgreText* text = new OgreText();
	//text->setText("Hello World!");
	//text->setTextPosition(00, 00);
	//text->setTextColour(1, 1, 1, 1);

	//OgreText* text2 = new OgreText();
	//text2->setText("TEXTO DONE WUEEE");
	//text2->setTextPosition(200, 200);
	//text2->setTextColour(.7, .1, 1, 1);

	//OgreText* text3 = new OgreText();
	//text3->setText("AAAAAAAAAAAAA");
	//text3->setTextPosition(500, 300);
	//text3->setTextColour(.1, .5, .8, 1);

	//graphical plane
	rsi->createPlane("planoo", Ogre::Vector3::UNIT_Y, 1000, 1000, Ogre::Vector3::UNIT_Y);
	rsi->setMaterial("planoo", "DebugMaterial");

	//ninja
	Ogre::SceneNode* nin = rsi->createOgreEntity("Ninja", "monoDebug.mesh").first;
	nin->setScale(500, 500, 500);
	nin->setPosition(0, -1000, 0);
	nin->pitch(Ogre::Radian(-3.14 / 2));
	rsi->setMaterial("Ninja", "BasicMaterial");
}

void TestComponent::setDown() {}

///////////////////////////////////////////////////////////////////////////////

void TestComponent::update(GameObject * ent, float time) {
	//LogSystem::Log();

	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = , boxS.getSize().y, boxS.getSize().z);
	//std::cout << boxS.getSize().x << std::endl;
}

bool TestComponent::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;
	//const float f = 100;

	//if (evt.type == SDL_KEYDOWN) {
	//	handled = true;

	//	switch (evt.key.keysym.sym) {
	//	case SDLK_r:
	//		rigidBodyD->setGlobalPose(PxTransform(PxVec3(0.0f, 5.0f, 0.0f)));
	//		rigidBodyD->setLinearVelocity(PxVec3(0.0f));
	//		break;

	//	case SDLK_w: rigidBodyD->addForce(f * -pZ); break;
	//	case SDLK_s: rigidBodyD->addForce(f * pZ); break;
	//	case SDLK_a: rigidBodyD->addForce(f * -pX); break;
	//	case SDLK_d: rigidBodyD->addForce(f * pX); break;
	//	case SDLK_c: rigidBodyD->addForce(f * -pY); break;
	//	case SDLK_SPACE: rigidBodyD->addForce(f * pY); break;

	//	default: handled = false; break;
	//	}
	//}

	return handled;
}

void TestComponent::late_update(GameObject * ent, float time) {}
