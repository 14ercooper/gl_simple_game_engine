
#include "Light.h"

Light::Light(LIGHT_TYPE type, float r, float g, float b, float intensity) {
	this->type = type;
	this->r = r;
	this->g = g;
	this->b = b;
	this->intensity = intensity;
	this->x = 0;
	this->y = -1;
	this->z = 0;
	this->doDestroy = false;
}

Light::Light(LIGHT_TYPE type, float r, float g, float b, float intensity, float x, float y, float z) {
	this->type = type;
	this->r = r;
	this->g = g;
	this->b = b;
	this->intensity = intensity;
	this->x = x;
	this->y = y;
	this->z = z;
	this->doDestroy = false;
}

void Light::setDestroy(bool value) {
	doDestroy = value;
}

bool Light::getDestroy() {
	return doDestroy;
}
