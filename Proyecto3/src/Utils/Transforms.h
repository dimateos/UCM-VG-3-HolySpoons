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

//ogre uses too high units as base scale
#define ogre_scale 100

//wraps physx and ogre vector3
class nap_vector3
{
public:
	//constructors
	inline nap_vector3() : x_(0.0f), y_(0.0f), z_(0.0f) {};
	inline nap_vector3(float f) : x_(f), y_(f), z_(f) {};
	inline nap_vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {};
	inline nap_vector3(nap_vector3 const & v) : x_(v.x_), y_(v.y_), z_(v.z_) {};

	inline nap_vector3(nlohmann::json const & v) : x_(v["x"]), y_(v["y"]), z_(v["z"]) {};
	//inline nap_vector3(Ogre::Vector3 const & v); //requires .h
	//inline nap_vector3(physx::PxVec3 const & v);

	//transformations
	Ogre::Vector3 ogre();
	physx::PxVec3 px();
	nlohmann::json json() { //could be used for easy debug
		return nlohmann::json({ {"x", x_}, {"y", y_}, {"z", z_} });
	};

	//some ops
	inline nap_vector3 operator+(const nap_vector3 & v2) {
		return nap_vector3(this->x_ + v2.x_, this->y_ + v2.y_, this->z_ + v2.z_);
	};
	inline nap_vector3 operator-(const nap_vector3 & v2) {
		return nap_vector3(this->x_ - v2.x_, this->y_ - v2.y_, this->z_ - v2.z_);
	};
	inline nap_vector3 operator*(float n) {
		return nap_vector3(this->x_ * n, this->y_ * n, this->z_ * n);
	};
	inline nap_vector3 operator*(const nap_vector3 & v2) {
		return nap_vector3(this->x_ * v2.x_, this->y_ * v2.y_, this->z_ * v2.z_);
	};

	float x_, y_, z_;
};

//wraps physx and ogre quaternions
class nap_quat
{
public:
	//constructors
	inline nap_quat() : w_(1.0f), x_(0.0f), y_(0.0f), z_(0.0f) {};
	inline nap_quat(float f) : w_(f), x_(f), y_(f), z_(f) {};
	inline nap_quat(float w, float x, float y, float z) : w_(w), x_(x), y_(y), z_(z) {};
	inline nap_quat(nap_quat const & q) : w_(q.w_), x_(q.x_), y_(q.y_), z_(q.z_) {};

	inline nap_quat(nlohmann::json const & q) : w_(q["w"]), x_(q["x"]), y_(q["y"]), z_(q["z"]) {};
	//inline nap_quat(Ogre::Quaternion const & v); //requires .h
	//inline nap_quat(physx::PxQuat const & v);

	//transformations
	Ogre::Quaternion ogre();
	physx::PxQuat px();
	nlohmann::json json() { //could be used for easy debug
		return nlohmann::json({ {"w", w_}, {"x", x_}, {"y", y_}, {"z", z_} });
	};
	nap_vector3 toNapVec3();

	//some ops
	inline nap_quat operator*(float n) {
		return nap_quat(this->w_ * n,
			this->x_ * n, this->y_ * n, this->z_ * n);
	};

	float w_, x_, y_, z_;
};

// base vectors
#define vX nap_vector3(1.0f, 0.0f, 0.0f)
#define vY nap_vector3(0.0f, 1.0f, 0.0f)
#define vZ nap_vector3(0.0f, 0.0f, 1.0f)
#define vO nap_vector3(0.0f, 0.0f, 0.0f)

// base quats
#define qO nap_quat(1.0f, 0.0f, 0.0f, 0.0f)

//struct for the scale (maybe inside transform?)
class nap_scale
{
public:
	inline nap_scale() : s_(1.0f) {};
	inline nap_scale(nap_vector3 s) : s_(s) {};

	bool upToDate_phys = false, upToDate_rend = false;
	nap_vector3 s_;
};

//struct for the gameObject transform
class nap_transform
{
public:
	inline nap_transform() : p_(), q_() {};
	inline nap_transform(nap_vector3 p) : p_(p), q_() {};
	inline nap_transform(nap_vector3 p, nap_quat q, nap_vector3 s) : p_(p), q_(q) {};

	bool upToDate_phys = false, upToDate_rend = false;
	nap_vector3 p_; nap_quat q_;
};

#endif /* TRANSFORMS_H_ */

