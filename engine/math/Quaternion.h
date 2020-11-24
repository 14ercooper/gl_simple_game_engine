
#ifndef QUATERNION_CLASS
#define QUATERNION_CLASS

#include <cmath>
#include <glm/glm.hpp>

class Quaternion {
public:
	float w, x, y, z;

	Quaternion();
	Quaternion(float w, float x, float y, float z);

	~Quaternion();

	void hamilton(Quaternion* other);
	void euler(float theta, float x, float y, float z);
	void normalize();
	void normaizeAxis();
	float magnitude();
	float magnitudeAxis();
	void invert();

	float rotationAngle();
	glm::vec3 rotationAxis();
};

#endif
