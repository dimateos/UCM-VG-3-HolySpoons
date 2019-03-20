#include "PhysicsComponent.h"
#include <iostream> //temp debug

#include <RenderSystemInterface.h>
#include <OgreText.h>

void PhysicsComponent::update(GameObject * ent, float time) {
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = , boxS.getSize().y, boxS.getSize().z);
	//std::cout << boxS.getSize().x << std::endl;
}

bool PhysicsComponent::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = false;
	const float f = 100;

	if (evt.type == SDL_KEYDOWN) {
		handled = true;

		switch (evt.key.keysym.sym) {
		case SDLK_r:
			rigidBodyD->setGlobalPose(PxTransform(PxVec3(0.0f, 5.0f, 0.0f)));
			rigidBodyD->setLinearVelocity(PxVec3(0.0f));
			break;

		case SDLK_w: rigidBodyD->addForce(f * -pZ); break;
		case SDLK_s: rigidBodyD->addForce(f * pZ); break;
		case SDLK_a: rigidBodyD->addForce(f * -pX); break;
		case SDLK_d: rigidBodyD->addForce(f * pX); break;
		case SDLK_c: rigidBodyD->addForce(f * -pY); break;
		case SDLK_SPACE: rigidBodyD->addForce(f * pY); break;

		default: handled = false; break;
		}
	}

	return handled;
}

void PhysicsComponent::setUp() {
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	RenderSystemInterface* rInterface = RenderSystemInterface::getSingleton();

	//auto light = renderInterface->createLight("light", Ogre::Light::LT_POINT, ColourValue(0.75f, 0.2f, 0.2f));
	//light->setPosition(-1.0f, 0.0f, 0.0f);

	//text
	OgreText* text = new OgreText();
	text->setText("Hello World!");
	text->setTextPosition(00, 00);
	text->setTextColour(1, 1, 1, 1);

	OgreText* text2 = new OgreText();
	text2->setText("TEXTO DONE WUEEE");
	text2->setTextPosition(200, 200);
	text2->setTextColour(.7, .1, 1, 1);

	OgreText* text3 = new OgreText();
	text3->setText("AAAAAAAAAAAAA");
	text3->setTextPosition(500, 300);
	text3->setTextColour(.1, .5, .8, 1);

	//graphical plane
	rInterface->createPlane("planoo", Vector3::UNIT_Y, 1000, 1000, Vector3::UNIT_Y);
	rInterface->setMaterial("planoo", "DebugMaterial");

	//static
	nodeS = rInterface->createOgreEntity("ground", "cube.mesh").first;
	rInterface->setMaterial("ground", "DebugMaterial2");
	nodeS->setScale(10.0f, 1.0f, 10.0f);
	nodeS->setPosition(0.0f, -1500.0f, 0.0f);
	PxGeometry *geoS = new PxBoxGeometry(PxVec3(10.0f, 1.0f, 10.0f) / 2); //half extents
	PxTransform transS(PxVec3(0.0f, -15.0f, 0.0f));
	rigidBodyS = physicsManager->createStaticBody(geoS, transS);
	//rigidBodyS->userData = nodeS; //static wont move

	//only after first ogre update...
	//auto boxSs = static_cast<Entity*>(nodeS->getAttachedObject("static"))->getWorldBoundingBox();
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = PxBoxGeometry(boxS.getSize().x, boxS.getSize().y, boxS.getSize().z);

	SceneNode* nin = rInterface->createOgreEntity("Ninja", "ninja.mesh").first;
	nin->setScale(10, 10, 10);
	nin->setPosition(0, -1000, 0);
	nin->yaw(Ogre::Radian(3.14159));
	rInterface->setMaterial("Ninja", "NinjaMaterial");

	//dynamic
	nodeD = rInterface->createOgreEntity("cube", "cube.mesh").first;
	rInterface->setMaterial("cube", "DebugMaterial2");
	PxGeometry *geoD = new PxBoxGeometry(PxVec3(1.0f) / 2);
	PxTransform transD(PxVec3(0.0f, 5.0f, 0.0f));
	rigidBodyD = physicsManager->createDynamicBody(geoD, transD);
	rigidBodyD->userData = nodeD;
}

void PhysicsComponent::setDown() {
	//release the bodies (which releases the shape etc)
	rigidBodyS->release();
	rigidBodyD->release();
}
