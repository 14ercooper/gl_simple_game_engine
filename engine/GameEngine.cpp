
#include "GameEngine.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdio>

// Create a new engine instance
GameEngine::GameEngine() {
	// Set up GLFW window and OpenGL context
	if (!glfwInit())
		fprintf(stderr, "[ERROR] GLFW could not be initialized.");

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	window = glfwCreateWindow(600, 400, "Default Window", nullptr, nullptr);

	if (!window)
		fprintf(stderr, "[ERROR] Could not create GLFW window");

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glEnable( GL_DEPTH_TEST );					                    // enable depth testing
	glDepthFunc( GL_LESS );							                // use less than depth test

	glEnable(GL_BLEND);									            // enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black
}

// Clean up our memory
GameEngine::~GameEngine() {
	// Delete the window
	glfwDestroyWindow(window);
	glfwTerminate();
}

// Draw the scene to the screen
bool GameEngine::render() {
	// Flush and swap buffers, trigger draw, and poll keypresses
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Return that we're still drawing
	return true;
}
