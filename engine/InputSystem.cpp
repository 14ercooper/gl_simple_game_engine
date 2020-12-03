
#include "InputSystem.h"
#include <cstdio>

void InputSystem::cursor_callback( GLFWwindow* window, double xPos, double yPos ) {
	if (mouseX == -99999) {
		// No cursor flipout
		mouseX = xPos;
		mouseY = yPos;
	}

	mouseAccel += glm::vec2(xPos - mouseX, yPos - mouseY);

	// printf("%f %f\n", mouseAccel.x, mouseAccel.y);

	mouseX = xPos;
	mouseY = yPos;
}

void InputSystem::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		mouse[button] = true;
	}
	else if (action == GLFW_RELEASE) {
		mouse[button] = false;
	}
}

void InputSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		keyboard[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		keyboard[key] = false;
	}
}

void InputSystem::scroll_callback(GLFWwindow* window, double xOffset, double yOffset ) {
	scrollOffset = glm::vec2(xOffset, yOffset);
}

glm::vec2 InputSystem::getMouseAcceleration() {
	return mouseAccel;
}

glm::vec2 InputSystem::getScrollOffset() {
	return scrollOffset;
}

bool InputSystem::isKeyPressed(int key) {
	return keyboard[key];
}

bool InputSystem::isMouseButtonPressed(int button) {
	return mouse[button];
}

void InputSystem::resetDynamicInputs() {
	mouseAccel = glm::vec2(0, 0);
	scrollOffset = glm::vec2(0, 0);
}
