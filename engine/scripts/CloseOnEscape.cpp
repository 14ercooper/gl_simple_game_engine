
#include "CloseOnEscape.h"

CloseOnEscape::CloseOnEscape() {
	shouldDestroy = false;
}

void** CloseOnEscape::run(void** args) {
	if (InputSystem::isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(GameEngine::engineWindow, GLFW_TRUE);
	}
	return (void**) 0;
}
