
#ifndef OUT_OF_BOUNDS_SCRIPT
#define OUT_OF_BOUNDS_SCRIPT

#include <glm/glm.hpp>

#include "engine/Script.h"
#include "engine/Object.h"
#include "engine/GameEngine.h"

class OutOfBounds : public Script {
public:
	OutOfBounds();

	void** run(void** args);

private:
	double getRand();
};

#endif
