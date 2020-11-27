
#ifndef CAMERA_CLASS
#define CAMERA_CLASS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>

class Camera; // Gotta love mutually dependent classes

#include "InputSystem.h"
#include "Object.h"

class Camera {
public:
	Camera();

	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 lookAt;

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(GLFWwindow* window);
	glm::vec3 getViewingVector();

	void setFollow(Object* obj, bool destroyOld);

	virtual void update();

protected:
	Object* follow;

private:
	float arcVert, arcRot, arcDist;
};

#endif
