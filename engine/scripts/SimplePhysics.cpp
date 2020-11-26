
#include "SimplePhysics.h"

void** SimplePhysics::run(void** args) {
	// Add gravity
	GameEngine::currentObject->velocity += glm::vec3(0.0f, -0.001f, 0.0f);

	// Clamp movement speed
	GameEngine::currentObject->velocity.x = clamp(GameEngine::currentObject->velocity.x, -2.0f, 2.0f);
	GameEngine::currentObject->velocity.y = clamp(GameEngine::currentObject->velocity.y, -0.1f, 2.0f);
	GameEngine::currentObject->velocity.z = clamp(GameEngine::currentObject->velocity.z, -2.0f, 2.0f);

	// Apply vertical quaterstep movement
	int vStepsDone = GameEngine::currentObject->quarterstepTranslate(glm::vec3(0.0f, GameEngine::currentObject->velocity.y, 0.0f));

	// Apply horizontal quaterstep movement
	int hStepsDone = GameEngine::currentObject->quarterstepTranslate(glm::vec3(GameEngine::currentObject->velocity.x, 0.0f, GameEngine::currentObject->velocity.z));

	// Reduce speed if we hit something
	GameEngine::currentObject->velocity.x *= (float) hStepsDone / 4.0f;
	GameEngine::currentObject->velocity.y *= (float) vStepsDone / 4.0f;
	GameEngine::currentObject->velocity.z *= (float) hStepsDone / 4.0f;

	return (void**) 0;
}

float SimplePhysics::clamp(float val, float min, float max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}
