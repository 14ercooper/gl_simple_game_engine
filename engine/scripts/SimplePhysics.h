
#ifndef SIMPLE_PHYSICS_SCRIPT
#define SIMPLE_PHYSICS_SCRIPT

#include <glm/glm.hpp>

#include "../GameEngine.h"
#include "../Script.h"

// Simple physics that applies gravity and moves the object, checking for collisions
class SimplePhysics : public Script {
public:
	SimplePhysics();

	void** run(void** args);

private:
	float clamp(float val, float min, float max);
};

#endif
