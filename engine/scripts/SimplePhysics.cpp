
#include "SimplePhysics.h"

SimplePhysics::SimplePhysics() {
	shouldDestroy = false;
}

void** SimplePhysics::run(void** args) {
	// Add gravity
	GameEngine::currentObject->velocity += glm::vec3(0.0f, -0.001f, 0.0f);

	// Clamp movement speed
	GameEngine::currentObject->velocity.x = clamp(GameEngine::currentObject->velocity.x, -2.0f, 2.0f);
	GameEngine::currentObject->velocity.y = clamp(GameEngine::currentObject->velocity.y, -0.35f, 2.0f);
	GameEngine::currentObject->velocity.z = clamp(GameEngine::currentObject->velocity.z, -2.0f, 2.0f);

	// Apply horizontal quaterstep movement
	GameEngine::currentObject->translate(glm::vec3(0.0f, 1.0f, 0.0f));
	int hStepsDone = GameEngine::currentObject->quarterstepTranslate(glm::vec3(GameEngine::currentObject->velocity.x, 0.0f, GameEngine::currentObject->velocity.z));
	GameEngine::currentObject->quarterstepTranslate(glm::vec3(0.0f, -1.0f, 0.0f));

	// Apply vertical quaterstep movement
	int vStepsDone = GameEngine::currentObject->quarterstepTranslate(glm::vec3(0.0f, GameEngine::currentObject->velocity.y, 0.0f));

	// Stop if we hit something
	if (hStepsDone != 4) {
		GameEngine::currentObject->velocity.x = 0.0f;
		GameEngine::currentObject->velocity.z = 0.0f;
	}
	if (vStepsDone != 4) {
		GameEngine::currentObject->velocity.y = 0.0f;
	}

	return (void**) 0;
}

float SimplePhysics::clamp(float val, float min, float max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}
