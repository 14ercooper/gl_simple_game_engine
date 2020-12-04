
#include "BouncePhysics.h"

BouncePhysics::BouncePhysics() {
	shouldDestroy = false;
}

double BouncePhysics::getRand() { return (2.0*(rand() / (double)RAND_MAX)) - 1; }

void** BouncePhysics::run(void** args) {
	// Add gravity
	GameEngine::currentObject->velocity += glm::vec3(0.0f, -0.002f, 0.0f);

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

	// If we hit something, stop, then if enemy or player, bounce
	std::vector<Object*> colls = GameEngine::engine->checkCollisions(GameEngine::currentObject->getCollider(), GameEngine::currentObject->velocity.x, GameEngine::currentObject->velocity.y, GameEngine::currentObject->velocity.z);
	if (hStepsDone != 16) {
		GameEngine::currentObject->velocity.x = 0.0f;
		GameEngine::currentObject->velocity.z = 0.0f;
		for (Object* obj : colls) {
			if (GameEngine::currentObject->hasTag("player") || GameEngine::currentObject->hasTag("enemy")) {
				if (obj->hasTag("player") || obj->hasTag("enemy")) {
					// Bounce this
					glm::vec3 newVel = glm::normalize(glm::vec3(getRand(), (getRand() + 1) / 2.0f, getRand())) * 0.55f;
					GameEngine::currentObject->velocity = newVel;
					GameEngine::currentObject->position = GameEngine::currentObject->position + newVel*3.0f;
					// Bounce other
					glm::vec3 newVel2 = glm::vec3(newVel.x * -1.0f, newVel.y, newVel.z * -1.0f);
					obj->velocity = newVel2;
					obj->position = obj->position + newVel2*3.0f;
				}
			}
		}
	}
	if (vStepsDone != 16) {
		GameEngine::currentObject->velocity.y = 0.0f;
		for (Object* obj : colls) {
			if (GameEngine::currentObject->hasTag("player") || GameEngine::currentObject->hasTag("enemy")) {
				if (obj->hasTag("player") || obj->hasTag("enemy")) {
					// Bounce this
					glm::vec3 newVel = glm::normalize(glm::vec3(getRand(), (getRand() + 1) / 2.0f, getRand())) * 0.55f;
					GameEngine::currentObject->velocity = newVel;
					GameEngine::currentObject->position = GameEngine::currentObject->position + newVel*3.0f;
					// Bounce other
					glm::vec3 newVel2 = glm::vec3(newVel.x * -1.0f, newVel.y, newVel.z * -1.0f);
					obj->velocity = newVel2;
					obj->position = obj->position + newVel2*3.0f;
				}
			}
		}
	}

	// Slow down horizontal movement over time
	GameEngine::currentObject->velocity.x *= 0.98f;
	GameEngine::currentObject->velocity.z *= 0.98f;

	return (void**) 0;
}

float BouncePhysics::clamp(float val, float min, float max) {
	if (val < min)
		return min;
	if (val > max)
		return max;
	return val;
}
