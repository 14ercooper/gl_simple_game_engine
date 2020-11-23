
#include "Object.h"

Object::Object() {
	material = new Material();
	collider = new Collider();
	model = new Model();

	physicsTickScript = new Script();
	controlTickScript = new Script();
	postTickScript = new Script();

	physicsTickData = (void **) 0;
	controlTickData = (void **) 0;
	postTickData = (void **) 0;

	transform = glm::mat4(1.0f);
}

Object::~Object() {
	delete material;
	delete collider;
	delete model;

	delete physicsTickScript;
	delete controlTickScript;
	delete postTickScript;
}

void Object::setShader(ShaderProgram* shader, bool deleteOld) {
	if (deleteOld)
		delete shaderProgram;

	shaderProgram = shader;
}

ShaderProgram* Object::getShader() {
	return shaderProgram;
}

void Object::setMaterial(Material* m, bool deleteOld) {
	if (deleteOld)
		delete material;
	material = m;
}

void Object::setCollider(Collider* c, bool deleteOld) {
	if (deleteOld)
		delete collider;
	collider = c;
}

void Object::setModel(Model* m, bool deleteOld) {
	if (deleteOld)
		delete model;
	model = m;
}

Material* Object::getMaterial() {
	return material;
}

Collider* Object::getCollider() {
	return collider;
}

Model* Object::getModel() {
	return model;
}

void Object::setPhysicsTick(Script* s, bool deleteOld) {
	if (deleteOld)
		delete physicsTickScript;
	physicsTickScript = s;
}

void Object::setControlTick(Script* s, bool deleteOld) {
	if (deleteOld)
		delete controlTickScript;
	controlTickScript = s;
}

void Object::setPostTick(Script* s, bool deleteOld) {
	if (deleteOld)
		delete postTickScript;
	postTickScript = s;
}

glm::mat4 Object::getTransform() {
	return transform;
}

void Object::setTransform(glm::mat4 trans) {
	this->transform = trans;
}

void Object::physicsTick() {
	physicsTickData = physicsTickScript->run((void **) 0);
}

void Object::controlTick() {
	controlTickData = controlTickScript->run((void **) 0);
}

void Object::postTick() {
	postTickData = postTickScript->run((void **) 0);
}

void Object::draw() {
	model->draw();
}

void Object::setDestroy(bool val) {
	shouldDestroy = val;
}

bool Object::getDestroy() {
	return shouldDestroy;
}
