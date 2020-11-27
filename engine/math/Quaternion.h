
#ifndef QUATERNION_CLASS
#define QUATERNION_CLASS

#include <cmath>
#include <glm/glm.hpp>

// An implementation of quaternions suitable for use as rotations
class Quaternion {
public:
	// Components of the quaternion
	float w, x, y, z;

	// Create a new quaternion, with optional components
	// Default constructor is no rotation around the positive X axis
	Quaternion();
	Quaternion(float w, float x, float y, float z);

	// Destroy this quaternion
	~Quaternion();

	// Update this quaternion so it represents it's current rotation followed by other's rotation
	void hamilton(Quaternion* other);

	// Replace the components of this quaternion with those representing a rotation of theta radians around the axis x,y,z
	void euler(float theta, float x, float y, float z);

	// Normalize/get magnitude of various components of this quaternion
	void normalize();
	void normaizeAxis();
	float magnitude();
	float magnitudeAxis();

	// Get the quaternion that, when hamilton'ed with this quaternion, returns no rotation.
	Quaternion* invert();

	// Get the angle and axis of rotation this quaternion represents
	float rotationAngle();
	glm::vec3 rotationAxis();
};

#endif
