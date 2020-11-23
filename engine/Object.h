
#ifndef OBJECT_CLASS
#define OBJECT_CLASS

#include <glm/glm.hpp>

#include "Material.h"
#include "Script.h"
#include "Model.h"
#include "Collider.h"

class Object {
public:
	Object();
	~Object();

	void setMaterial(Material* m);
	void setCollider(Collider* c);
	void setModel(Model* m);

	void setPhysicsTick(Script* s);
	void setControlTick(Script* s);
	void setPostTick(Script* s);

	glm::mat4 getTransform();
	void setTransform(glm::mat4 transform);

	void physicsTick();
	void controlTick();
	void postTick();

	void draw();

private:
	Material* material;
	Collider* collider;
	Model* model;

	Script* physicsTickScript;
	Script* controlTickScript;
	Script* postTickScript;

	void** physicsTickData;
	void** controlTickData;
	void** postTickData;

	glm::mat4 transform;
};

#endif
