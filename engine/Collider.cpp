
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
	if (other == nullptr)
		return false;

	// Do AABB check
	// Get ranges
	float thisXMin, thisXMax, thisYMin, thisYMax, thisZMin, thisZMax;
	float otherXMin, otherXMax, otherYMin, otherYMax, otherZMin, otherZMax;
	thisXMin = thisYMin = thisZMin = -1.0f;
	thisXMax = thisYMax = thisZMax = 1.0f;
	otherXMin = otherYMin = otherZMin = -1.0f;
	otherXMax = otherYMax = otherZMax = 1.0f;
	if (follow != nullptr) {
		thisXMin = this->follow->position.x + testX - (this->x * this->follow->currentScale.x);
		thisXMax = this->follow->position.x + testX + (this->x * this->follow->currentScale.x);
		thisYMin = this->follow->position.y + testY - (this->y * this->follow->currentScale.y);
		thisYMax = this->follow->position.y + testY + (this->y * this->follow->currentScale.y);
		thisZMin = this->follow->position.z + testZ - (this->z * this->follow->currentScale.z);
		thisZMax = this->follow->position.z + testZ + (this->z * this->follow->currentScale.z);
	}
	if (other->follow != nullptr) {
		otherXMin = other->follow->position.x + testX - (other->x * other->follow->currentScale.x);
		otherXMax = other->follow->position.x + testX + (other->x * other->follow->currentScale.x);
		otherYMin = other->follow->position.y + testY - (other->y * other->follow->currentScale.y);
		otherYMax = other->follow->position.y + testY + (other->y * other->follow->currentScale.y);
		otherZMin = other->follow->position.z + testZ - (other->z * other->follow->currentScale.z);
		otherZMax = other->follow->position.z + testZ + (other->z * other->follow->currentScale.z);
	}

	// X axis
	bool xOverlap = thisXMin <= otherXMax && thisXMax >= otherXMin;

	// Y axis
	bool yOverlap = thisYMin <= otherYMax && thisYMax >= otherYMin;

	// Z axis
	bool zOverlap = thisZMin <= otherZMax && thisZMax >= otherZMin;

	// Do the and and return
	return xOverlap && yOverlap && zOverlap;
}
