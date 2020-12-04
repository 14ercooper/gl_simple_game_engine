
#include "OutOfBoundsScript.h"

double OutOfBounds::getRand() { return (2.0*(rand() / (double)RAND_MAX)) - 1; }

OutOfBounds::OutOfBounds() {
	shouldDestroy = false;
}

void** OutOfBounds::run(void** args) {
	Object* obj = GameEngine::currentObject;
	bool boundsTriggered = false;

	// X bounds
	if (obj->position.x > 60.0f)
		boundsTriggered = true;
	if (obj->position.x < -60.0f)
		boundsTriggered = true;

	// Z bounds
	if (obj->position.z > 60.0f)
		boundsTriggered = true;
	if (obj->position.z < -60.0f)
		boundsTriggered = true;

	// Y bounds
	if (obj->position.y > 60.0f)
		boundsTriggered = true;
	if (obj->position.y < -60.0f)
		boundsTriggered = true;

	// Set position if bounds triggered
	if (boundsTriggered) {
		if (obj->hasTag("player")) {
			obj->position = glm::vec3(0.0f, 7.5f, 0.0f);
			obj->velocity = glm::vec3(0.0f, 1.25f, 0.0f);
			GameEngine::engineTimer = 0;
		}

		// Randomize enemy velocity if bounds triggered
		if (obj->hasTag("enemy")) {
			obj->position = glm::vec3(getRand() * 45.0f, 25.0f, getRand() * 45.0f);
			obj->velocity = glm::vec3(getRand(), 0.0f, getRand());
		}
	}

	return (void**) 0;
}
