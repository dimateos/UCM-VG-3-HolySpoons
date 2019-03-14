#include "PhysicsComponent.h"
#include <iostream> //temp debug

#include "RenderSystemInterface.h"

void PhysicsComponent::update(GameObject * ent, float time) {
	//auto boxS = nodeS->_getWorldAABB();
	//PxGeometry geoS = , boxS.getSize().y, boxS.getSize().z);
	//std::cout << boxS.getSize().x << std::endl;
}

bool PhysicsComponent::handleEvents(GameObject * ent, const SDL_Event & evt) {
	bool handled = true;
	const float f = 100;

	if (evt.type == SDL_KEYDOWN) {
		switch (evt.key.keysym.sym) {
		case SDLK_r:
			rigidBodyD->setGlobalPose(PxTransform (PxVec3(0.0f, 5.0f, 0.0f)));
			rigidBodyD->setLinearVelocity(PxVec3(0.0f));
			break;

		case SDLK_w: rigidBodyD->addForce(f * -vZ); break;
		case SDLK_s: rigidBodyD->addForce(f * vZ); break;
		case SDLK_a: rigidBodyD->addForce(f * -vX); break;
		case SDLK_d: rigidBodyD->addForce(f * vX); break;
		case SDLK_c: rigidBodyD->addForce(f * -vY); break;
		case SDLK_SPACE: rigidBodyD->addForce(f * vY); break;

		default: handled = false; break;
		}
	}

	return handled;
}

void PhysicsComponent::setUp() {
	PhysicsSystemManager* physicsManager = PhysicsSystemManager::getSingleton();
	RenderSystemInterface* renderInterface = RenderSystemInterface::getSingleton();

	//cam
	renderInterface->getCameraNode()->setPosition(500, 500, 4000);
	renderInterface->getCameraNode()->lookAt(Ogre::Vector3(0.0f, -500.0f, 0.0f), Ogre::Node::TS_WORLD);
	//auto light = renderInterface->createLight("light", Ogre::Light::LT_POINT, ColourValue(0.75f, 0.2f, 0.2f));
	//light->setPosition(-1.0f, 0.0f, 0.0f);

	//static
	nodeS = renderInterface->createOgreEntity("ground", "cube.mesh").first;
	renderInterface->setMaterial("ground", "Examples/OgreLogo");
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
	renderInterface->createPlane("planoo", Vector3::UNIT_Y, 1000, 1000,Vector3::UNIT_Y);


	//dynamic
	nodeD = renderInterface->createOgreEntity("cube", "cube.mesh").first;
	renderInterface->setMaterial("cube", "WoodPallet");
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
