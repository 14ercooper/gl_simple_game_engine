
#ifndef OBJECT_CLASS
#define OBJECT_CLASS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdlib>

#include "Material.h"
#include "Script.h"
#include "Model.h"
#include "Collider.h"
#include "ShaderProgram.h"
#include "math/Quaternion.h"

class Object {
public:
	Object();
	~Object();

	glm::vec3 position;
	Quaternion* rotation;
	glm::vec3 currentScale;

	void translate(glm::vec3 amount);
	void rotate(Quaternion* rotation);
	void rotate(float theta, float x, float y, float z);
	void scale(glm::vec3 amount);

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

	void physicsTick();
	void controlTick();
	void postTick();

	virtual void draw();

	void setDestroy(bool val);
	bool getDestroy();

	glm::mat4 getModelMatrix();

protected:
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

	bool shouldDestroy;
};

#endif
