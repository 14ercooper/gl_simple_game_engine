
#include "Camera.h"

Camera::Camera() {
	pos = glm::vec3(5.0f, 2.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(pos, lookAt, up);
}

glm::mat4 getProjectionMatrix(GLFWwindow* window) {
    GLint windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );
    return glm::perspective( 45.0f, (GLfloat) windowWidth / (GLfloat) windowHeight, 0.001f, 1000.0f );
}
