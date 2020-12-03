
#include "SimpleMovement.h"

SimpleMovement::SimpleMovement() {
	shouldDestroy = false;
	hasJumped = false;
	isGrounded = false;
}

void** SimpleMovement::run(void** args) {
	// Get inputs
	bool up = InputSystem::isKeyPressed(GLFW_KEY_W);
	bool down = InputSystem::isKeyPressed(GLFW_KEY_S);
	bool right = InputSystem::isKeyPressed(GLFW_KEY_D);
	bool left = InputSystem::isKeyPressed(GLFW_KEY_A);
	bool jump = InputSystem::isKeyPressed(GLFW_KEY_SPACE);

	// glm::vec3 forwardVec = GameEngine::currentObject->position - GameEngine::engineCamera->pos;
	glm::vec3 forwardVec = GameEngine::engineCamera->getViewingVector() * -1.0f;
	forwardVec.y = 0;
	glm::vec3 rightVec = glm::cross(forwardVec, glm::vec3(0.0f, 1.0f, 0.0f));

	// printf("%f %f %f\n", rightVec.x, rightVec.y, rightVec.z);

	// Check grounded (for jump)
	if (GameEngine::engine->raycast(GameEngine::currentObject->position, glm::vec3(0, -1, 0), GameEngine::currentObject->currentScale.y * 1.1f, 100, GameEngine::currentObject->getCollider(), false) == nullptr) {
		isGrounded = false;
	}
	else {
		isGrounded = true;
	}

	// Reset jump if valid
	if (hasJumped && isGrounded) {
		hasJumped = false;
	}

	// Apply horizontal movement
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	if (up)
		movement = movement + (forwardVec);
	if (down)
		movement = movement + (-1.0f * forwardVec);
	if (right)
		movement = movement + (rightVec);
	if (left)
		movement = movement + (-1.0f * rightVec);
	if (glm::length(movement) > 0.01f) {
		movement = glm::normalize(movement) * 0.003f; // Movement speed
	}
	GameEngine::currentObject->addVelocity(movement);

	// Apply jump
	if (jump && !hasJumped && isGrounded) {
		GameEngine::currentObject->translate(glm::vec3(0, 0.1f, 0));
		GameEngine::currentObject->velocity.y = 0.22f;
		hasJumped = true;
	}

	return (void**) 0;
}
