
#include "SimplePhysics.h"

SimplePhysics::SimplePhysics() {
	shouldDestroy = false;
}

void** SimplePhysics::run(void** args) {
	// Add gravity
	GameEngine::currentObject->velocity += glm::vec3(0.0f, -0.001f, 0.0f);

	// Clamp movement speed
	GameEngine::currentObject->velocity.x = clamp(GameEngine::currentObject->velocity.x, -0.35f, 0.35f);
	GameEngine::currentObject->velocity.y = clamp(GameEngine::currentObject->velocity.y, -0.35f, 0.35f);
	GameEngine::currentObject->velocity.z = clamp(GameEngine::currentObject->velocity.z, -0.35f, 0.35f);

	// Apply vertical quaterstep movement
	int vStepsDone = GameEngine::currentObject->quarterstepTranslateRay(glm::vec3(0.0f, GameEngine::currentObject->velocity.y, 0.0f), glm::vec3(0, -GameEngine::currentObject->currentScale.y, 0));

	// Apply horizontal quaterstep movement
	glm::vec3 horizontalMove = glm::vec3(GameEngine::currentObject->velocity.x, 0.0f, GameEngine::currentObject->velocity.z);
	GameEngine::currentObject->translate(glm::vec3(0.0f, 0.1f, 0.0f));
	int hStepsDone = GameEngine::currentObject->quarterstepTranslate(horizontalMove);
	GameEngine::currentObject->quarterstepTranslateRay(glm::vec3(0.0f, -0.2f, 0.0f), glm::vec3(0, -1, 0));

	// Stop if we hit something
	if (hStepsDone != 16) {
		GameEngine::currentObject->velocity.x = 0.0f;
		GameEngine::currentObject->velocity.z = 0.0f;
	}
	if (vStepsDone != 16) {
		GameEngine::currentObject->velocity.y = 0.0f;
	}

	// Slow down horizontal movement over time
	GameEngine::currentObject->velocity.x *= 0.98f;
	GameEngine::currentObject->velocity.z *= 0.98f;

	return (void**) 0;
}

float SimplePhysics::clamp(float val, float min, float max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}
