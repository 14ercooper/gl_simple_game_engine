
#include "Camera.h"

Camera::Camera() {
	pos = glm::vec3(5.0f, 2.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix() {
	// return glm::lookAt(pos, lookAt, up); // TODO figure out why this segfaults
	return glm::lookAt( glm::vec3(5.0f, 2.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(GLFWwindow* window) {
    GLint windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    return glm::perspective( 45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight, 0.001f, 1000.0f );
}

glm::vec3 Camera::getViewingVector() {
	// return glm::normalize(pos - lookAt); // TODO this likely errors for the same reason as above
	return glm::normalize(glm::vec3(5.0f, 2.0f, 0.0f) - glm::vec3(0.0f, 0.0f, 0.0f));
}
