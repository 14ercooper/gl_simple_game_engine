
#ifndef SIMPLE_MOVEMENT_SCRIPT
#define SIMPLE_MOVEMENT_SCRIPT

#include "../Script.h"

class SimpleMovement : public Script {
public:
	SimpleMovement();

	void** run(void** args);
};

#endif
