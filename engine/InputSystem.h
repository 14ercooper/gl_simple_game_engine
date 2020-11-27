
#ifndef INPUT_SYSTEM
#define INPUT_SYSTEM

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace InputSystem {
	// Callbacks for the window. Don't need to touch there
	void cursor_callback( GLFWwindow* window, double xPos, double yPos );
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void scroll_callback(GLFWwindow* window, double xOffset, double yOffset );

	// Get inputs
	// Mouse acceleration is mouse delta from last frame in pixels
	// Ints are the GLFW key codes/enums
	glm::vec2 getMouseAcceleration();
	glm::vec2 getScrollOffset();
	bool isKeyPressed(int key);
	bool isMouseButtonPressed(int button);

	// Prevent things like mosueAccel and scrollOffset from getting stuck if we don't hit the callbacks
	void resetDynamicInputs();

	// Keep track of different input events
	static bool keyboard[GLFW_KEY_LAST];
	static bool mouse[GLFW_MOUSE_BUTTON_LAST];
	static double mouseX = -99999, mouseY = -99999;
	static glm::vec2 mouseAccel = glm::vec2(0, 0), scrollOffset = glm::vec2(0, 0);
}

#endif
