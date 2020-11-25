
#include "Camera.h"

Camera::Camera() {
	this->pos = glm::vec3(5.0f, 2.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix() {
	// printf("%p\n", &pos);
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
