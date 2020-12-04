
#include "Enemy.h"

EnemyObject::EnemyObject (Object* chase) {
	// Load shader
	shaderProgram = new ColorGouradShader();

	// Load material
	material = new MatteSkyBlue();

	// Load model
	model = new ColoredCube();

	// Set collider
	setCollider(new Collider(this), false);

	// Set the chase
	this->chase = chase;

	// Add the enemy tag
	velocity = glm::vec3(0, 0, 0);
	addTag("enemy");
}

EnemyObject::~EnemyObject() {
	delete shaderProgram;
}

void EnemyObject::draw() {
	// Do the heading update if grounded, else stop moving
	bool isGrounded = GameEngine::engine->raycast(GameEngine::currentObject->position, glm::vec3(0, -1, 0), GameEngine::currentObject->currentScale.y * 1.1f, 100, GameEngine::currentObject->getCollider(), false) != nullptr;
	if (!isGrounded) {
		if (glm::length(glm::vec2(velocity.x, velocity.z)) > 0.01f) {
			storedHeading.x = velocity.x;
			storedHeading.y = velocity.z;
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	else {
		if (glm::length(velocity) < 0.01f) {
			velocity.x = storedHeading.x;
			velocity.z = storedHeading.y;
		}
		float yV = velocity.y;
		velocity.y = 0;
		const float MAX_SPEED = 0.1f;

		if (glm::length(velocity) < 0.001f) {
			velocity = glm::vec3(1, 0, 0);
		}

		// Get the axis and 
		glm::vec3 vVec = glm::normalize(this->velocity);
		glm::vec3 gVec = glm::normalize(glm::normalize(chase->position - this->position));
		float angle = acos(glm::dot(vVec, gVec));
		glm::vec3 axis = glm::cross(vVec, gVec);

		// Compute the rotation matrix
		glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), angle * 0.014f, axis);

		// Do the math to update velocity
		glm::vec4 vel = glm::vec4(velocity, 1.0f);
		vel = rotMat * vel;
		velocity = glm::normalize(glm::vec3(vel)) * MAX_SPEED;

		if (velocity.x > MAX_SPEED)
			velocity.x = MAX_SPEED;
		if (velocity.z > MAX_SPEED)
			velocity.x = MAX_SPEED;
		if (velocity.x < -MAX_SPEED)
			velocity.x = -MAX_SPEED;
		if (velocity.z < -MAX_SPEED)
			velocity.x = -MAX_SPEED;

		velocity.y = yV;
	}

	// Use shader
	shaderProgram->useProgram();

	GameEngine::engineShaderProgram = shaderProgram;

	// Buffer mvp uniforms
	shaderProgram->uniformMat4("modelMatrix", getModelMatrix());
	shaderProgram->uniformMat4("viewMatrix", GameEngine::engineCamera->getViewMatrix());
	shaderProgram->uniformMat4("projectionMatrix", GameEngine::engineCamera->getProjectionMatrix(GameEngine::engineWindow));

	// Buffer material uniforms
	shaderProgram->uniformVec3("materialColor", glm::vec3(material->r, material->g, material->b));
	shaderProgram->uniformFloat("materialDiffuse", material->diffuse);
	shaderProgram->uniformFloat("materialSpecularity", material->specular);

	// Buffer viewing vector uniform
	shaderProgram->uniformVec3("viewingVector", GameEngine::engineCamera->getViewingVector());

	material->use();

	// Draw model
	model->draw();
}
