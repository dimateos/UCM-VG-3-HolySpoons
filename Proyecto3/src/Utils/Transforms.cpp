#include "Transforms.h"

//maybe better macros?

#include <OgreVector3.h>
Ogre::Vector3 nap_vector3::ogre() {
	return Ogre::Vector3(x_, y_, z_);
}
#include <OgreQuaternion.h>
Ogre::Quaternion nap_quat::ogre() {
	return Ogre::Quaternion(w_, x_, y_, z_);
}

#include <PxRigidActor.h>
physx::PxVec3 nap_vector3::px() {
	return physx::PxVec3(x_, y_, z_);
}
physx::PxQuat nap_quat::px() {
	return physx::PxQuat(w_, x_, y_, z_);
}
