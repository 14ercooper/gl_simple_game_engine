
#include "GameEngine.h"

GLFWwindow* GameEngine::engineWindow = nullptr;
Camera* GameEngine::engineCamera = nullptr;
ShaderProgram* GameEngine::engineShaderProgram = nullptr;

void error_callback(int error, const char* description) {
    fprintf(stderr, "[ERROR]: (%d) %s\n", error, description);
}

void cursor_callback( GLFWwindow* window, double xPos, double yPos ) {}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {}
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset ) {}

// Create a new engine instance
GameEngine::GameEngine() {
	glfwSetErrorCallback(error_callback);

	// Set up GLFW window and OpenGL context
	if (!glfwInit()) {
		fprintf(stderr, "[ERROR] GLFW could not be initialized.");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );                               // do not allow the window to be resized


	window = glfwCreateWindow(600, 400, "Default Window", nullptr, nullptr);

	if (!window) {
		fprintf(stderr, "[ERROR] Could not create GLFW window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(         window, key_callback		  );            	// set our keyboard callback function
    glfwSetMouseButtonCallback( window, mouse_button_callback );	            // set our mouse button callback function
    glfwSetCursorPosCallback(	window, cursor_callback  	  );	            // set our cursor position callback function
    glfwSetScrollCallback(		window, scroll_callback		  );	            // set our scroll wheel callback function

	glewExperimental = GL_TRUE;
    GLenum glewResult = glewInit();

    // check for an error
    if( glewResult != GLEW_OK ) {
        fprintf( stderr, "[ERROR]: Error initializing GLEW\n");
        fprintf( stderr, "[ERROR]: %s\n", glewGetErrorString(glewResult) );
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	glEnable( GL_DEPTH_TEST );					                    // enable depth testing
	glDepthFunc( GL_LESS );							                // use less than depth test

	glEnable(GL_BLEND);									            // enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	            // use one minus blending equation

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );	// clear the frame buffer to black

	objects = std::vector<Object*>();
	camera = new Camera();
}

// Clean up our memory
GameEngine::~GameEngine() {
	// Delete the window
	glfwDestroyWindow(window);
	glfwTerminate();

	delete camera;
}

void GameEngine::purgeObjects() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects.at(i);
	}
	objects = std::vector<Object*>();
}

// Draw the scene to the screen
bool GameEngine::render() {
    glDrawBuffer( GL_BACK );				        // work with our back frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// clear the current color contents and depth buffer in the window

    GLint windowWidth, windowHeight;
    glfwGetFramebufferSize( window, &windowWidth, &windowHeight );

    // update the viewport - tell OpenGL we want to render to the whole window
    glViewport( 0, 0, windowWidth, windowHeight );
	
	// Update statics
	GameEngine::engineWindow = this->window;
	GameEngine::engineCamera = this->engineCamera;

	// Tick physics
	for (Object* obj : objects) {
		obj->physicsTick();
	}

	// Tick control
	for (Object* obj : objects) {
		obj->controlTick();
	}

	// Draw
	for (Object* obj : objects) {
		obj->draw();
	}

	// Tick post
	for (Object* obj : objects) {
		obj->postTick();
	}

	// Delete any objects that have expired
	for (int i = 0; i < objects.size(); i++) {
		if (objects.at(i)->getDestroy()) {
			Object* obj = objects.at(i);
			objects.erase(objects.begin() + i);
			delete obj;
			i--; // Don't skip over any objects
		}
	}

	// Flush and swap buffers, trigger draw, and poll keypresses
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Return that we're still drawing
	return !glfwWindowShouldClose(window);
}

void GameEngine::addObject(Object* o) {
	objects.push_back(o);
}
