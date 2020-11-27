
#ifndef CLOSE_ESC_SCRIPT
#define CLOSE_ESC_SCRIPT

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Script.h"
#include "../InputSystem.h"
#include "../GameEngine.h"

// Engine script that closes the window when the escape key is pressed
class CloseOnEscape : public Script {
public:
	CloseOnEscape();

	void** run(void** args);
};

#endif
