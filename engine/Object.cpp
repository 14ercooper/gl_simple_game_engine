
#include <glm/glm.hpp>

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

void Object::setMaterial(Material* m) {
	delete material;
	material = m;
}

void Object::setCollider(Collider* c) {
	delete collider;
	collider = c;
}

void Object::setModel(Model* m) {
	delete model;
	model = m;
}

void Object::setPhysicsTick(Script* s) {
	delete physicsTickScript;
	physicsTickScript = s;
}

void Object::setControlTick(Script* s) {
	delete controlTickScript;
	controlTickScript = s;
}

void Object::setPostTick(Script* s) {
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
