#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

//foward declarations
namespace Ogre
{
	template <int dims, typename T> class Vector;
	typedef float Real;
	typedef Vector<3, Real> Vector3;
	class Quaternion;
}
namespace physx
{
	class PxTransform;
	class PxVec3;
	class PxQuat;
}

//parse jsons
#include "json.hpp"

//wraps physx and ogre vector3
class nap_vector3
{
public:
	inline nap_vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {};
	inline nap_vector3(float f) : x_(f), y_(f), z_(f) {};
	inline nap_vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {};
	inline nap_vector3(nap_vector3 const & v) : x_(v.x_), y_(v.y_), z_(v.z_) {};
	inline nap_vector3(nlohmann::json const & v) : x_(v["x"]), y_(v["y"]), z_(v["z"]) {};

	float x_, y_, z_;
	Ogre::Vector3 ogre();
	physx::PxVec3 px();
};

//wraps physx and ogre quaternions
class nap_quat
{
public:
	inline nap_quat() : w_(1.0f), x_(0.0f), y_(0.0f), z_(0.0f) {};
	inline nap_quat(float f) : w_(f), x_(f), y_(f), z_(f) {};
	inline nap_quat(float w, float x, float y, float z) : w_(w), x_(x), y_(y), z_(z) {};
	inline nap_quat(nap_quat const & q) : w_(q.w_), x_(q.x_), y_(q.y_), z_(q.z_) {};
	inline nap_quat(nlohmann::json const & q) : w_(q["w"]), x_(q["x"]), y_(q["y"]), z_(q["z"]) {};

	float w_, x_, y_, z_;
	Ogre::Quaternion ogre();
	physx::PxQuat px();
};

// base vectors
#define vX nap_vector3(1.0f, 0.0f, 0.0f)
#define vY nap_vector3(0.0f, 1.0f, 0.0f)
#define vZ nap_vector3(0.0f, 0.0f, 1.0f)
#define vO nap_vector3(0.0f, 0.0f, 0.0f)

// base quats
#define vO nap_vector3(1.0f, 0.0f, 0.0f, 0.0f)

//struct for the gameObject transform
enum updateState { upToDate, pxUpdated, userUpdated };
class nap_transform
{
public:
	inline nap_transform() : updateState_(userUpdated), p_(), q_() {};
	inline nap_transform(nap_vector3 p) : updateState_(userUpdated), p_(p), q_() {};
	inline nap_transform(nap_vector3 p, nap_quat q) : updateState_(userUpdated), p_(p), q_(q) {};

	updateState updateState_;
	nap_vector3 p_; nap_quat q_;
};

#endif /* TRANSFORMS_H_ */

