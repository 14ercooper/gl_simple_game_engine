
#ifndef COLLIDER_CLASS
#define COLLIDER_CLASS

#include <set>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

#include "Object.h"
#include "math/Quaternion.h"

struct CollisionPoint {
	float x, y, z;
};

class Collider {
public:
	Collider();
	Collider(Object* follow);
	Collider(Object* follow, float xSize, float ySize, float zSize);
	~Collider();

	void setFollow(Object* follow, bool deleteOld);
	void setSize(float xSize, float ySize, float zSize);

	bool isColliding(Collider* other);
	bool isTriggered(Collider* other);
	bool isColliding(Collider* other, float testX, float testY, float testZ);
	bool isTriggered(Collider* other, float testX, float testY, float testZ);

	bool pointInCollider(glm::vec3 point);

	void recalcTransforms();

	glm::mat4 fromWorldSpace;
	glm::mat4 toWorldSpace;

	bool trackParentRotation;
	Quaternion* rotation;

	bool isTrigger;
	std::set<std::string> tags;
protected:
	Object* follow;
	float x, y, z;

	bool collideHelper(Collider* other, float testX, float testY, float testZ);
	bool pointInBox(glm::mat4 spaceToSpaceTransform, CollisionPoint point);
	bool inMiddleRange(float val);
};

#endif
