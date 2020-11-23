
#ifndef OBJECT_CLASS
#define OBJECT_CLASS

#include <glm/glm.hpp>

#include "Material.h"
#include "Script.h"
#include "Model.h"
#include "Collider.h"
#include "ShaderProgram.h"

class Object {
public:
	Object();
	~Object();

	void setShader(ShaderProgram* program, bool deleteOld);
	ShaderProgram* getShader();

	void setMaterial(Material* m, bool deleteOld);
	void setCollider(Collider* c, bool deleteOld);
	void setModel(Model* m, bool deleteOld);

	Material* getMaterial();
	Collider* getCollider();
	Model* getModel();

	void setPhysicsTick(Script* s, bool deleteOld);
	void setControlTick(Script* s, bool deleteOld);
	void setPostTick(Script* s, bool deleteOld);

	glm::mat4 getTransform();
	void setTransform(glm::mat4 transform);

	void physicsTick();
	void controlTick();
	void postTick();

	void draw();

	void setDestroy(bool val);
	bool getDestroy();

private:
	ShaderProgram* shaderProgram;

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

	bool shouldDestroy;
};

#endif
