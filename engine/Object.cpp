
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

	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = new Quaternion();
	currentScale = glm::vec3(1.0f, 1.0f, 1.0f);

	shouldDestroy = false;
}

Object::~Object() {
	delete material;
	delete collider;
	delete model;

	delete physicsTickScript;
	delete controlTickScript;
	delete postTickScript;

	// delete rotation; // TODO figure out why this crashes
}

void Object::setShader(ShaderProgram* shader, bool deleteOld) {
	if (deleteOld)
		delete shaderProgram;

	shaderProgram = shader;
}

ShaderProgram* Object::getShader() {
	return shaderProgram;
}

void Object::translate(glm::vec3 amount) {
	position += amount;
}

void Object::rotate(Quaternion* rotation) {
	this->rotation->hamilton(rotation);
}

void Object::rotate(float theta, float x, float y, float z) {
	Quaternion* rot = new Quaternion();
	rot->euler(theta, x, y, z);
	this->rotation->hamilton(rot);
	delete rot;
}

void Object::scale(glm::vec3 amount) {
	currentScale *= amount;
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

glm::mat4 Object::getModelMatrix() {
	glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
	matrix = glm::rotate(matrix, rotation->rotationAngle(), rotation->rotationAxis());
	matrix = glm::scale(matrix, currentScale);
	return matrix;
}
