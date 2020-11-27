
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

// A camera. The default style is arcball (provided by this class)
class Camera {
public:
	// Create a new camera
	Camera();

	// The position, look at location, and up vector of the camera
	glm::vec3 pos;
	glm::vec3 lookAt;
	glm::vec3 up;

	// Gets the view matrix, look at matrix, and viewing vector of this camera
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix(GLFWwindow* window);
	glm::vec3 getViewingVector();

	// Set what object the camera should "follow". If not set, default to 0,0,0
	void setFollow(Object* obj, bool destroyOld);

	// Update the camera's three vectors
	virtual void update();

protected:
	// The object being followed
	Object* follow;

private:
	// Data for the arcball
	float arcVert, arcRot, arcDist;
};

#endif
