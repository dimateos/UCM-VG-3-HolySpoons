#include "RenderComponent.h"
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include "LogSystem.h"
#include "GOFactory.h"

void RenderComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the ogrePair with the node and the correct entity built
	auto pair = getOgrePair(cfg_["shape"]);
	node = pair.first;
	entity = pair.second;

	//other properties
	if (FIND(cfg_, "scale")) node->setScale(nap_vector3(cfg_["scale"]).ogre());
	if (FIND(cfg_, "material")) entity->setMaterialName(cfg_["material"]);

	//node->showBoundingBox(true); //global config
	//node->flipVisibility();''

	configActive();
}

void RenderComponent::setDown() {
	node->removeAndDestroyAllChildren();
	node->getCreator()->destroySceneNode(node);
}

void RenderComponent::configActive() {
	node->setVisible(active_);
}

///////////////////////////////////////////////////////////////////////////////

OgrePair RenderComponent::getOgrePair(nap_json shape) {
	RenderSystemInterface* rsi = RenderSystemInterface::getSingleton();
	OgrePair pair;

	//switch the type to construct the correct shape
	string type = shape["type"];
	if (type == "MESH") {
		string e_name = shape["entity_name"];
		pair = rsi->createOgreEntity( e_name + id().sn_string(), shape["mesh"]);
	}

	return pair;
}

///////////////////////////////////////////////////////////////////////////////

void RenderComponent::late_update(GameObject * o, double time) {
	if (o->getTransUpToDate_rend()) return;

	node->setPosition(o->getPosition().ogre() * ogre_scale);
	node->setOrientation(o->getOrientation().ogre());
	o->setTransUpToDate_rend();
}

REGISTER_TYPE(RenderComponent);
