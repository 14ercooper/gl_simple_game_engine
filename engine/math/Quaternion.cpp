
#include "Quaternion.h"

Quaternion::Quaternion () {
	x = 1.0f;
	w = y = z = 0.0f;
}

Quaternion::Quaternion(float w, float x, float y, float z) {
	this->w = w;
	this->x = x;
	this->y = y;
	this->z = z;
}

Quaternion::~Quaternion() {
	
}

void Quaternion::hamilton (Quaternion* other) {
	this->w = (w*other->w) - (x*other->x) - (y*other->y) - (z*other->z);
	this->x = (w*other->x) + (x*other->w) + (y*other->z) - (z*other->y);
	this->y = (w*other->y) - (x*other->z) + (y*other->w) + (z*other->x);
	this->z = (w*other->z) + (x*other->y) - (y*other->x) + (z*other->w);
}

void Quaternion::euler(float theta, float x, float y, float z) {
	this->w = cos(theta/2.0f);
	float halfSin = sin(theta/2.0f);
	this->x = x*halfSin;
	this->y = y*halfSin;
	this->z = z*halfSin;
}

void Quaternion::invert() {
	x *= -1.0f;
	y *= -1.0f;
	z *= -1.0f;
}

void Quaternion::normalize() {
	float mag = magnitude();
	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
}

void Quaternion::normaizeAxis() {
	float mag = magnitudeAxis();
	x /= mag;
	y /= mag;
	z /= mag;
}

float Quaternion::magnitude() {
	return sqrt((w*w) + (x*x) + (y*y) + (z*z));
}

float Quaternion::magnitudeAxis() {
	return sqrt((x*x) + (y*y) + (z*z));
}

float Quaternion::rotationAngle() {
	return 2 * atan2(magnitudeAxis(), w);
}

glm::vec3 Quaternion::rotationAxis() {
	normaizeAxis();
	return glm::vec3(x, y, z);
}
