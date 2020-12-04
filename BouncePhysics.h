
#ifndef BOUNCE_PHYSICS_SCRIPT
#define BOUNCE_PHYSICS_SCRIPT

#include <glm/glm.hpp>

#include <vector>

#include "engine/GameEngine.h"
#include "engine/Script.h"
#include "engine/Object.h"

// Bounce physics that applies gravity and moves the object, checking for collisions
class BouncePhysics : public Script {
public:
	BouncePhysics();

	void** run(void** args);

private:
	float clamp(float val, float min, float max);
	double getRand();
};

#endif
