
#include "GameEngine.h"

GameEngine* GameEngine::engine = nullptr;
GLFWwindow* GameEngine::engineWindow = nullptr;
Camera* GameEngine::engineCamera = nullptr;
ShaderProgram* GameEngine::engineShaderProgram = nullptr;
Object* GameEngine::currentObject = nullptr;

void error_callback(int error, const char* description) {
    fprintf(stderr, "[ERROR]: (%d) %s\n", error, description);
}

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

	glfwSetKeyCallback(         window, InputSystem::key_callback		  );            	// set our keyboard callback function
    glfwSetMouseButtonCallback( window, InputSystem::mouse_button_callback );	            // set our mouse button callback function
    glfwSetCursorPosCallback(	window, InputSystem::cursor_callback  	  );	            // set our cursor position callback function
    glfwSetScrollCallback(		window, InputSystem::scroll_callback		  );	            // set our scroll wheel callback function

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

	secondPassShader = nullptr;
	skyboxShader = nullptr;

	doneCollisionCalc = false;
}

// Clean up our memory
GameEngine::~GameEngine() {
	// Delete the window
	glfwDestroyWindow(window);
	glfwTerminate();

	delete camera;
}

void GameEngine::setWindowSize(int width, int height) {
	if (width < 1 || height < 1) {
		// Full screen
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, 19200, 10800, GLFW_DONT_CARE);
	}
	else {
		// Windowed
		glfwSetWindowMonitor(window, NULL, 0, 0, width, height, GLFW_DONT_CARE);
	}
}

void GameEngine::purgeObjects() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects.at(i);
	}
	objects.clear();
}

void GameEngine::setSecondPass(SecondPassShader* shader, bool destroyOld) {
	if (destroyOld && secondPassShader != nullptr)
		delete secondPassShader;
	secondPassShader = shader;
}

void GameEngine::setSkyboxShader(SkyboxShader* shader, bool destroyOld) {
	if (destroyOld && skyboxShader != nullptr)
		delete skyboxShader;
	skyboxShader = shader;
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
	GameEngine::engine = this;
	GameEngine::engineWindow = this->window;
	GameEngine::engineCamera = this->camera;

	// Calc collisions
	doCollisionCalc();

	// Tick control
	for (Object* obj : objects) {
		currentObject = obj;
		obj->controlTick();
	}

	// Tick physics
	for (Object* obj : objects) {
		currentObject = obj;
		obj->physicsTick();
	}

	// If using second pass, use it
	if (secondPassShader != nullptr) {
		secondPassShader->setDrawToBuffer();
	}

	// If we have a skybox, draw it
	if (skyboxShader != nullptr)
		skyboxShader->drawSkybox();

	// Draw
	for (Object* obj : objects) {
		currentObject = obj;
		obj->draw();
	}

	// If using second pass, draw it to the screen
	if (secondPassShader != nullptr) {
		secondPassShader->drawBufferToScreen();
	}

	// Tick post
	for (Object* obj : objects) {
		currentObject = obj;
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

	// Run engine scripts
	for (Script* s : engineScripts) {
		s->run((void**) 0);
	}

	// Delete expired engine scripts
	for (int i = 0; i < engineScripts.size(); i++) {
		if (engineScripts.at(i)->getDestroy()) {
			Script* obj = engineScripts.at(i);
			engineScripts.erase(engineScripts.begin() + i);
			delete obj;
			i--; // Don't skip over any scripts
		}
	}

	// Flush and swap buffers, trigger draw, and poll keypresses
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Reset the collisions flag
	doneCollisionCalc = false;
	InputSystem::resetDynamicInputs();

	// Return that we're still drawing
	return !glfwWindowShouldClose(window);
}

void GameEngine::addObject(Object* o) {
	objects.push_back(o);
}

void GameEngine::addScript(Script* s) {
	engineScripts.push_back(s);
}

std::vector<Object*> GameEngine::checkCollisions(Collider* c) {
	std::vector<Object*> collisionsFound;

	for (Object* o : colliders) {
		if (o->getCollider() == nullptr)
			continue;
		if (c->isColliding(o->getCollider())) {
			collisionsFound.push_back(o);
		}
	}

	return collisionsFound;
}

std::vector<Object*> GameEngine::checkTriggers(Collider* c) {
	std::vector<Object*> collisionsFound;

	for (Object* o : triggers) {
		if (o->getCollider() == nullptr)
			continue;
		if (c->isTriggered(o->getCollider())) {
			collisionsFound.push_back(o);
		}
	}

	return collisionsFound;
}

std::vector<Object*> GameEngine::checkCollisions(Collider* c, float testX, float testY, float testZ) {
	std::vector<Object*> collisionsFound;

	for (Object* o : colliders) {
		if (o->getCollider() == nullptr)
			continue;
		if (c->isColliding(o->getCollider(), testX, testY, testZ)) {
			collisionsFound.push_back(o);
		}
	}

	return collisionsFound;
}

std::vector<Object*> GameEngine::checkTriggers(Collider* c, float testX, float testY, float testZ) {
	std::vector<Object*> collisionsFound;

	for (Object* o : triggers) {
		if (o->getCollider() == nullptr)
			continue;
		if (c->isTriggered(o->getCollider(), testX, testY, testZ)) {
			collisionsFound.push_back(o);
		}
	}

	return collisionsFound;
}

void GameEngine::doCollisionCalc() {
	if (doneCollisionCalc)
		return;

	// Make sure this doesn't destroy the objects
	colliders.erase(colliders.begin(), colliders.begin() + colliders.size());
	triggers.erase(triggers.begin(), triggers.begin() + triggers.size());

	// Go through objects and build up the lists
	for (int i = 0; i < objects.size(); i++) {
		if (objects.at(i)->getCollider() == nullptr)
			continue;
		else if (objects.at(i)->getCollider()->isTrigger)
			triggers.push_back(objects.at(i));
		else
			colliders.push_back(objects.at(i));
	}

	doneCollisionCalc = true;
}
