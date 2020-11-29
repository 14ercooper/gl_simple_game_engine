
#include "Object.h"

Object::Object() {
	material = new Material();
	collider = nullptr;
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
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);

	shouldDestroy = false;

	parent = nullptr;

	delMat = delCol = delMod = delPhys = delCont = delPost = delShader = true;
}

Object::~Object() {
	if (delMat)
		delete material;
	if (delCol)
		delete collider;
	if (delMod)
		delete model;

	if (delPhys)
		delete physicsTickScript;
	if (delCont)
		delete controlTickScript;
	if (delPost)
		delete postTickScript;

	// delete rotation; // TODO figure out why this crashes
}

void Object::setShader(ShaderProgram* shader, bool deleteOld) {
	if (deleteOld)
		delete shaderProgram;
	delShader = false;
	shaderProgram = shader;
}

ShaderProgram* Object::getShader() {
	return shaderProgram;
}

void Object::translate(glm::vec3 amount) {
	position += amount;
}

int Object::quarterstepTranslate(glm::vec3 amount) {
	if (collider == nullptr || collider->isTrigger) {
		position += amount;
		return 16;
	}
	glm::vec3 quaterstep = amount * 0.25f * 0.25f;
	for (int i = 0; i < 16; i++) {
		if (GameEngine::engine->checkCollisions(collider, quaterstep.x, quaterstep.y, quaterstep.z).size() == 0) {
			position += quaterstep;
		}
		else {
			// Prevent getting stuck
			for (int i = 0; i < 32; i++) {
				position -= quaterstep;
				if (GameEngine::engine->checkCollisions(collider).size() == 0)
					break;
			}
			return i;
		}
	}
	return 16;
}

int Object::quarterstepTranslateRay(glm::vec3 amount, glm::vec3 offset) {
	if (collider == nullptr || collider->isTrigger) {
		position += amount;
		return 16;
	}
	glm::vec3 quaterstep = amount * 0.25f * 0.25f;
	float dist = glm::length(quaterstep);

	for (int i = 0; i < 16; i++) {
		if (GameEngine::engine->raycast(this->position + offset, quaterstep, dist, 100, this->getCollider(), false) == nullptr) {
			position += quaterstep;
		}
		else {
			return i;
		}
	}
	return 16;
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

void Object::addVelocity(glm::vec3 amount) {
	velocity += amount;
}

void Object::setMaterial(Material* m, bool deleteOld) {
	if (deleteOld)
		delete material;
	delMat = false;
	material = m;
}

void Object::setCollider(Collider* c, bool deleteOld) {
	if (deleteOld)
		if (collider != nullptr)
			delete collider;
	delCol = false;
	collider = c;
}

void Object::setModel(Model* m, bool deleteOld) {
	if (deleteOld)
		delete model;
	delMod = false;
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
	delPhys = false;
	physicsTickScript = s;
}

void Object::setControlTick(Script* s, bool deleteOld) {
	if (deleteOld)
		delete controlTickScript;
	delCont = false;
	controlTickScript = s;
}

void Object::setPostTick(Script* s, bool deleteOld) {
	if (deleteOld)
		delete postTickScript;
	delPost = false;
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
	glm::mat4 parentMatrix = glm::mat4(1.0f);
	if (parent != nullptr)
		parentMatrix = parent->getModelMatrix();
	glm::mat4 matrix = glm::translate(parentMatrix, position);
	matrix = glm::rotate(matrix, rotation->rotationAngle(), rotation->rotationAxis());
	matrix = glm::scale(matrix, currentScale);
	return matrix;
}

void Object::setParent(Object* newParent, bool destroyOld) {
	if (destroyOld && parent != nullptr)
		delete parent;
	parent = newParent;
}

Object* Object::getParent() {
	return parent;
}

void Object::addTag(std::string tag) {
	tags.insert(tag);
}

void Object::removeTag(std::string tag) {
	tags.erase(tags.find(tag));
}

bool Object::hasTag(std::string tag) {
	return tags.count(tag) > 0;
}
