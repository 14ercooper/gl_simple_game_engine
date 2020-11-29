
#ifndef SIMPLE_MOVEMENT_SCRIPT
#define SIMPLE_MOVEMENT_SCRIPT

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "../GameEngine.h"
#include "../InputSystem.h"
#include "../Script.h"

// A simple 3D movement script that can be attached to an object to make a "Player"
class SimpleMovement : public Script {
public:
	SimpleMovement();

	void** run(void** args);

private:
	bool hasJumped;
	bool isGrounded;
};

#endif
