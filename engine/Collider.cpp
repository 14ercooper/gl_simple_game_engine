
#include "Collider.h"

Collider::Collider() {
	follow = nullptr;
	x = y = z = 1.0f;
	isTrigger = false;
}

Collider::Collider(Object* follow) {
	this->follow = follow;
	x = y = z = 1.0f;
	isTrigger = false;
}

Collider::Collider(Object* follow, float xSize, float ySize, float zSize) {
	this->follow = follow;
	x = xSize;
	y = ySize;
	z = zSize;
	isTrigger = false;
}

void Collider::setFollow(Object* follow, bool destroyOld) {
	if (destroyOld)
		delete this->follow;
	this->follow = follow;
}

void Collider::setSize(float xSize, float ySize, float zSize) {
	x = xSize;
	y = ySize;
	z = zSize;
}

bool Collider::isColliding(Collider* other) {
	// No self collide
	if (other == this) {
		return false;
	}

	// Don't collide with triggers
	if (other->isTrigger)
		return false;

	return collideHelper(other, 0, 0, 0);
}

bool Collider::isTriggered(Collider* other) {
	// No self collide
	if (other == this) {
		return false;
	}

	// Don't collide with triggers
	if (!other->isTrigger)
		return false;

	return collideHelper(other, 0, 0, 0);
}

bool Collider::isColliding(Collider* other, float testX, float testY, float testZ) {
	// No self collide
	if (other == this) {
		return false;
	}

	// Don't collide with triggers
	if (other->isTrigger)
		return false;

	return collideHelper(other, testX, testY, testZ);
}

bool Collider::isTriggered(Collider* other, float testX, float testY, float testZ) {
	// No self collide
	if (other == this) {
		return false;
	}

	// Don't collide with triggers
	if (!other->isTrigger)
		return false;

	return collideHelper(other, testX, testY, testZ);
}

bool Collider::collideHelper(Collider* other, float testX, float testY, float testZ) {
	// Do AABB check
	// Get ranges
	float thisXMin, thisXMax, thisYMin, thisYMax, thisZMin, thisZMax;
	thisXMin = thisYMin = thisZMin = -1.0f;
	thisXMax = thisYMax = thisZMax = 1.0f;
	if (follow != nullptr) {
		thisXMin = this->follow->position.x + testX - this->x;
		thisXMax = this->follow->position.x + testX + this->x;
		thisYMin = this->follow->position.y + testY - this->y;
		thisYMax = this->follow->position.y + testY + this->y;
		thisZMin = this->follow->position.z + testZ - this->z;
		thisZMax = this->follow->position.z + testZ + this->z;
		thisXMax *= this->follow->currentScale.x;
		thisXMin *= this->follow->currentScale.x;
		thisYMax *= this->follow->currentScale.y;
		thisYMin *= this->follow->currentScale.y;
		thisZMax *= this->follow->currentScale.z;
		thisZMin *= this->follow->currentScale.z;
	}
	float otherXMin = other->follow->position.x - other->x;
	float otherXMax = other->follow->position.x + other->x;
	float otherYMin = other->follow->position.y - other->y;
	float otherYMax = other->follow->position.y + other->y;
	float otherZMin = other->follow->position.z - other->z;
	float otherZMax = other->follow->position.z + other->z;

	// X axis
	bool xOverlap = thisXMin <= otherXMax && thisXMax >= otherXMin;

	// Y axis
	bool yOverlap = thisYMin <= otherYMax && thisYMax >= otherYMin;

	// Z axis
	bool zOverlap = thisZMin <= otherZMax && thisZMax >= otherZMin;

	// Do the and and return
	return xOverlap && yOverlap && zOverlap;
}
