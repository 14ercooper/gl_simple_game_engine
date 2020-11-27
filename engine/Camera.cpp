
#include "Camera.h"

Camera::Camera() {
	this->pos = glm::vec3(5.0f, 2.0f, 0.0f);
	this->lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	follow = nullptr;

	arcVert = 1.0f;
	arcRot = 0.0f;
	arcDist = 20.0f;
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(pos, lookAt, up);
}

glm::mat4 Camera::getProjectionMatrix(GLFWwindow* window) {
    GLint windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    return glm::perspective( 45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight, 0.001f, 1000.0f );
}

glm::vec3 Camera::getViewingVector() {
	return glm::normalize(pos - lookAt);
}

void Camera::update() {
	float CAM_SENSITIVITY = 0.01f;
	glm::vec2 cameraUpdateAmount = glm::vec2(0, 0);
	if (InputSystem::isMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT)) {
		cameraUpdateAmount = InputSystem::getMouseAcceleration() * CAM_SENSITIVITY;
	}

	// Update and clamp/wrap look vars
	arcVert -= cameraUpdateAmount.y;
	arcRot += cameraUpdateAmount.x;

	float halfPi = 3.1414f * 0.5f; // Or I guess a hair less so we don't line up with up
	if (arcVert > halfPi)
		arcVert = halfPi;
	else if (arcVert < -halfPi)
		arcVert = -halfPi;

	while (arcRot > 6.28318f)
		arcRot -= 6.28318f;
	while (arcRot < -6.28318f)
		arcRot += 6.28318f;

	// Update the data vectors
	if (follow != nullptr)
		lookAt = follow->position;
	else
		lookAt = glm::vec3(0.0f, 0.0f, 0.0f);

	// Get direction
	glm::vec3 camVector = glm::normalize(glm::vec3(-cos(arcRot), -sin(-arcVert), sin(arcRot)));

	// Update the camera pos
	pos = lookAt + (arcDist * camVector);
}
