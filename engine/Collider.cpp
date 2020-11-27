
#include "Collider.h"

Collider::Collider() {
	follow = nullptr;
	x = y = z = 1.0f;
	isTrigger = false;
	trackParentRotation = false;
	rotation = new Quaternion();
	recalcTransforms();
}

Collider::Collider(Object* follow) {
	this->follow = follow;
	x = y = z = 1.0f;
	isTrigger = false;
	trackParentRotation = false;
	rotation = new Quaternion();
	recalcTransforms();
}

Collider::Collider(Object* follow, float xSize, float ySize, float zSize) {
	this->follow = follow;
	x = xSize;
	y = ySize;
	z = zSize;
	isTrigger = false;
	trackParentRotation = false;
	rotation = new Quaternion();
	recalcTransforms();
}

Collider::~Collider() {
	delete rotation;
}

void Collider::setFollow(Object* follow, bool destroyOld) {
	if (destroyOld)
		delete this->follow;
	this->follow = follow;
	recalcTransforms();
}

void Collider::setSize(float xSize, float ySize, float zSize) {
	x = xSize;
	y = ySize;
	z = zSize;
	recalcTransforms();
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

void Collider::recalcTransforms() {
	fromWorldSpace = glm::mat4(1.0f);
	if (follow != nullptr) {
		if (trackParentRotation) {
			fromWorldSpace = glm::translate(fromWorldSpace, follow->position);
			fromWorldSpace = glm::rotate(fromWorldSpace, follow->rotation->rotationAngle(), follow->rotation->rotationAxis());
			fromWorldSpace = glm::scale(fromWorldSpace, follow->currentScale);
		}
		else {
			fromWorldSpace = glm::translate(fromWorldSpace, follow->position);
			fromWorldSpace = glm::scale(fromWorldSpace, follow->currentScale);
		}
	}
	fromWorldSpace = glm::rotate(fromWorldSpace, rotation->rotationAngle(), rotation->rotationAxis());
	fromWorldSpace = glm::scale(fromWorldSpace, glm::vec3(x, y, z));
	toWorldSpace = glm::inverse(fromWorldSpace);
}

bool Collider::collideHelper(Collider* other, float testX, float testY, float testZ) {
	float x = testX;
	float y = testY;
	float z = testZ;

	// Create the 8 points and the space to space transform
	CollisionPoint points[] = {
		{1 + x, 1 + y, 1 + z},
		{1 + x, 1 + y, -1 + z},
		{1 + x, -1 + y, 1 + z},
		{1 + x, -1 + y, -1 + z},
		{-1 + x, 1 + y, 1 + z},
		{-1 + x, 1 + y, -1 + z},
		{-1 + x, -1 + y, 1 + z},
		{-1 + x, -1 + y, -1 + z},
	};

	glm::mat4 spaceToSpace = other->toWorldSpace * this->fromWorldSpace;

	// Eval on each point (until we hit a collision or check all 8 points)
	for (CollisionPoint p : points) {
		if (pointInBox(spaceToSpace, p))
			return true;
	}

	return false;
}

bool Collider::pointInBox(glm::mat4 spaceToSpaceTransform, CollisionPoint point) {
	glm::vec4 pt = glm::vec4(point.x, point.y, point.z, 1.0f);
	pt = spaceToSpaceTransform * pt;
	glm::vec3 outPt = glm::vec3(pt) / pt.w;
	bool val = inMiddleRange(outPt.x) && inMiddleRange(outPt.y) && inMiddleRange(outPt.z);
	return val;
}

bool Collider::inMiddleRange(float val) {
	return val >= -1.0f && val <= 1.0f;
}
