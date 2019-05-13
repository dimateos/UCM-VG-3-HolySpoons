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
	if (FIND(cfg_, "relativePos")) relativePos_ = nap_vector3(cfg_["relativePos"]);
	else relativePos_ = vO;
	if (FIND(cfg_, "localRot")) node->setOrientation(nap_quat(cfg_["localRot"]).ogre());
	if (FIND(cfg_, "material")) entity->setMaterialName(cfg_["material"]);

	//visibility
	invisible_ = FINDnRETURN(cfg_, "invisible", bool, false);

	//update trans
	ignoreTrans_ = FINDnRETURN(cfg_, "ignoreTrans", bool, false);
	updateOri_ = FINDnRETURN(cfg_, "updateOri", bool, true);

	//idle animation (if it has one)
	if (FIND(cfg_, "idleAnimation")) {
		string kk = cfg_["idleAnimation"];
		/*Ogre::String kk2;
		Ogre::AnimationStateSet *set = entity->getAllAnimationStates();
		Ogre::AnimationStateIterator iter = set->getAnimationStateIterator();
		while (iter.hasMoreElements())
		{
			kk2 = iter.getNext()->getAnimationName();
		}*/
		if (entity->hasAnimationState(kk)) {
			idleAnimation = entity->getAnimationState(kk);
			idleAnimation->setEnabled(true);
			idleAnimation->setLoop(true);
		}
	}

	configActive();
}

void RenderComponent::setDown() {
	node->removeAndDestroyAllChildren();
	node->getCreator()->destroySceneNode(node);
}

void RenderComponent::configActive() {
	node->setVisible(active_ && !invisible_);
	if (FIND(cfg_, "boundingBox")) node->showBoundingBox(active_ && cfg_["boundingBox"]);
	node->setPosition((owner_->getPosition().ogre() + relativePos_.ogre()) * ogre_scale);
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
	else if (type == "PLANE") {
		float auxWidth = shape["w"], auxHeight = shape["h"];
		pair = rsi->createPlane(e_name + id().sn_string(), nap_vector3(shape["normal"]).ogre(),
			auxWidth * ogre_scale, auxHeight * ogre_scale, nap_vector3(shape["up"]).ogre());
	}

	return pair;
}

///////////////////////////////////////////////////////////////////////////////

void RenderComponent::update(GameObject * o, double time) {
	if (idleAnimation != nullptr) idleAnimation->addTime(time);
}

void RenderComponent::late_update(GameObject * o, double time) {
	if (ignoreTrans_) return;

	//check pos
	if (!o->getUpToDate(upToDate::pos, upToDate::REND)) {
		node->setPosition((o->getPosition().ogre() + relativePos_.ogre()) * ogre_scale);
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
