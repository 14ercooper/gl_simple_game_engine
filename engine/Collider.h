
#ifndef COLLIDER_CLASS
#define COLLIDER_CLASS

#include <set>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

#include "Object.h"
#include "math/Quaternion.h"

// Just a way to wrap up 3 floats
struct CollisionPoint {
	float x, y, z;
};

// An implementation of a rotatable 3D cube collider
class Collider {
public:
	// Create a new collider
	// The default follow is 0,0,0 with a size of 1,1,1
	Collider();
	Collider(Object* follow);
	Collider(Object* follow, float xSize, float ySize, float zSize);
	~Collider();

	// Set what this collider should follow (be attached to) and it's size
	void setFollow(Object* follow, bool deleteOld);
	void setSize(float xSize, float ySize, float zSize);

	// Check if this collider is intersecting other
	// Test values are for predictive collisions (doesn't move, but will tell you if moving there will cause a collision)
	bool isColliding(Collider* other);
	bool isTriggered(Collider* other);
	bool isColliding(Collider* other, float testX, float testY, float testZ);
	bool isTriggered(Collider* other, float testX, float testY, float testZ);

	// Check if a point is within this collider
	bool pointInCollider(glm::vec3 point);

	// Recalc the two world space transform matricies (since they are not the fastest to compute - default every frame in GameEngine)
	void recalcTransforms();

	// Transforms for points to and from colliderspace to worldspace
	glm::mat4 fromWorldSpace;
	glm::mat4 toWorldSpace;

	// Should we track our parent (follow)'s rotation, as well what rotation we have
	bool trackParentRotation;
	Quaternion* rotation;

	// Are we a trigger or a solid collider?
	bool isTrigger;

	// What tags do we have?
	std::set<std::string> tags;
protected:
	// Follow object as well as size
	Object* follow;
	float x, y, z;

	// Helpers for detecting collisions
	bool collideHelper(Collider* other, float testX, float testY, float testZ);
	bool pointInBox(glm::mat4 spaceToSpaceTransform, CollisionPoint point);
	bool inMiddleRange(float val);
};

#endif
