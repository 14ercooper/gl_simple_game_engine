
#ifndef SIMPLE_PHYSICS_SCRIPT
#define SIMPLE_PHYSICS_SCRIPT

#include <glm/glm.hpp>

#include "../GameEngine.h"
#include "../Script.h"

class SimplePhysics : public Script {
public:
	void** run(void** args);

private:
	float clamp(float val, float min, float max);
};

#endif
