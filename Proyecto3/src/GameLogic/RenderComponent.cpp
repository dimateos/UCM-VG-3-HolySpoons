#include "RenderComponent.h"
#include "LogSystem.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>

#include <Transforms.h>

void RenderComponent::setUp() {
	if (isInited()) return;
	setInited();

	//get the ogrePair with the node and the correct entity built
	OgrePair pair;
	if (FIND(cfg_, "shape")) pair = getOgrePair(cfg_["shape"]);
	else LogSystem::Log("Shape not found on " + id().name_ + " component", LogSystem::REND);
	node = pair.first;
	entity = pair.second;

	//other properties
	if (FIND(cfg_, "scale")) node->setScale(nap_vector3(cfg_["scale"]).ogre());
	if (FIND(cfg_, "material")) entity->setMaterialName(cfg_["material"]);
	updateOri_ = FINDnRETURN(cfg_, "updateOri", bool, true);

	//visibility
	if (FIND(cfg_, "boundingBox")) node->showBoundingBox(cfg_["boundingBox"]);
	invisible_ = FINDnRETURN(cfg_, "invisible", bool, false);

	configActive();
}

void RenderComponent::setDown() {
	node->removeAndDestroyAllChildren();
	node->getCreator()->destroySceneNode(node);
}

void RenderComponent::configActive() {
	node->setVisible(active_ && !invisible_);
}

///////////////////////////////////////////////////////////////////////////////

OgrePair RenderComponent::getOgrePair(nap_json shape) {
	RenderSystemInterface* rsi = RenderSystemInterface::getSingleton();
	OgrePair pair;
	string e_name = shape["entity_name"];

	//switch the type to construct the correct shape
	string type = shape["type"];
	if (type == "EMPTY") pair.first = rsi->createEmpty(e_name + id().sn_string());
	else if (type == "MESH") pair = rsi->createOgreEntity(e_name + id().sn_string(), shape["mesh"]);

	return pair;
}

///////////////////////////////////////////////////////////////////////////////

void RenderComponent::late_update(GameObject * o, double time) {
	if (updateOri_ && o->getUpToDate_trans(upToDate::REND)) return;
	else if (o->getUpToDate(upToDate::pos, upToDate::REND)) return;

	node->setPosition(o->getPosition().ogre() * ogre_scale);
	if (updateOri_) node->setOrientation(o->getOrientation().ogre());

	if (updateOri_) o->setUpToDate_trans(upToDate::REND);
	else o->setUpToDate(upToDate::pos, upToDate::REND);
}

#include "GOFactory.h"
REGISTER_TYPE(RenderComponent);
