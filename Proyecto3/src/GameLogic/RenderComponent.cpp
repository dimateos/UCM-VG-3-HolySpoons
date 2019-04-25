#include "RenderComponent.h"
#include "LogSystem.h"

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreAnimationState.h>

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
	if (FIND(cfg_, "relativePos")) relativePos = nap_vector3(cfg_["relativePos"]);
	if (FIND(cfg_, "localRot")) 
		node->setOrientation(nap_quat(cfg_["localRot"]["w"], cfg_["localRot"]["x"], cfg_["localRot"]["y"], cfg_["localRot"]["z"]).ogre());
	else relativePos = nap_vector3(0, 0, 0);
	if (FIND(cfg_, "material")) entity->setMaterialName(cfg_["material"]);

	//visibility
	if (FIND(cfg_, "boundingBox")) node->showBoundingBox(cfg_["boundingBox"]);
	invisible_ = FINDnRETURN(cfg_, "invisible", bool, false);

	//update trans
	ignoreTrans_ = FINDnRETURN(cfg_, "ignoreTrans", bool, false);
	updateOri_ = FINDnRETURN(cfg_, "updateOri", bool, true);

	//idle animation (if it has one)
	if (FIND(cfg_, "idleAnimation") && entity->hasAnimationState(cfg_["idleAnimation"])) {
		idleAnimation = entity->getAnimationState(cfg_["idleAnimation"]);
		idleAnimation->setEnabled(true);
		idleAnimation->setLoop(true);
	}

	configActive();
}

void RenderComponent::setDown() {
	node->removeAndDestroyAllChildren();
	node->getCreator()->destroySceneNode(node);
}

void RenderComponent::configActive() {
	node->setVisible(active_ && !invisible_);
	if (!active_ && FIND(cfg_, "boundingBox")) node->showBoundingBox(!cfg_["boundingBox"]);
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

void RenderComponent::update(GameObject * o, double time) {
	if (idleAnimation != nullptr) idleAnimation->addTime(time);
}

void RenderComponent::late_update(GameObject * o, double time) {
	if (ignoreTrans_) {
		node->setPosition(relativePos.ogre() * ogre_scale);
		return;
	}

	//check pos
	if (!o->getUpToDate(upToDate::pos, upToDate::REND)) {
		node->setPosition((o->getPosition().ogre() + relativePos.ogre()) * ogre_scale);
		o->setUpToDate(upToDate::pos, upToDate::REND);
	}

	//now check the orientation if interested
	if (updateOri_ && !o->getUpToDate(upToDate::ori, upToDate::REND)) {
		node->setOrientation(o->getOrientation().ogre());
		o->setUpToDate(upToDate::ori, upToDate::REND);
	}
}

#include "GOFactory.h"
REGISTER_TYPE(RenderComponent);
