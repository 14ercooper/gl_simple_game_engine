
#ifndef COLLIDER_CLASS
#define COLLIDER_CLASS

#include <set>
#include <string>

#include "Object.h"

struct CollisionPoint {
	float x, y, z;
};

class Collider {
public:
	Collider();
	Collider(Object* follow);
	Collider(Object* follow, float xSize, float ySize, float zSize);

	void setFollow(Object* follow, bool deleteOld);
	void setSize(float xSize, float ySize, float zSize);

	bool isColliding(Collider* other);
	bool isTriggered(Collider* other);
	bool isColliding(Collider* other, float testX, float testY, float testZ);
	bool isTriggered(Collider* other, float testX, float testY, float testZ);

	bool isTrigger;
	std::set<std::string> tags;
protected:
	Object* follow;
	float x, y, z;

	bool collideHelper(Collider* other, float testX, float testY, float testZ);
};

#endif
