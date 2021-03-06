
#include "GameEngine.h"

GameEngine* GameEngine::engine = nullptr;
GLFWwindow* GameEngine::engineWindow = nullptr;
Camera* GameEngine::engineCamera = nullptr;
ShaderProgram* GameEngine::engineShaderProgram = nullptr;
Object* GameEngine::currentObject = nullptr;
long long unsigned int GameEngine::engineTimer = 0;

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
    //Hides cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	
	GameEngine::engine = this;
	GameEngine::engineWindow = this->window;
	GameEngine::engineCamera = this->camera;

	timer = 0;
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

void GameEngine::renameWindow(const char* name) {
	glfwSetWindowTitle(window, name);
}

void GameEngine::purgeObjects() {
	for (Object* obj : objects) {
		delete obj;
	}
	objects.erase(objects.begin(), objects.begin() + objects.size());
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

void GameEngine::setCamera(Camera* cam, bool destroyOld) {
	if (destroyOld)
		delete camera;
	camera = cam;
	GameEngine::engineCamera = camera;
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
	GameEngine::engineTimer = ++timer;
	GameEngine::engine = this;
	GameEngine::engineWindow = this->window;
	GameEngine::engineCamera = this->camera;

	// Calc collisions
	doCollisionCalc();

	// Update camera
	camera->update();

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

	// Tick post
	for (Object* obj : objects) {
		currentObject = obj;
		obj->postTick();
	}

	// Delete any objects that have expired
	for (long unsigned int i = 0; i < objects.size(); i++) {
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
	for (long unsigned int i = 0; i < engineScripts.size(); i++) {
		if (engineScripts.at(i)->getDestroy()) {
			Script* obj = engineScripts.at(i);
			engineScripts.erase(engineScripts.begin() + i);
			delete obj;
			i--; // Don't skip over any scripts
		}
	}

	// Return that we're still drawing
	return !glfwWindowShouldClose(window);
}

void GameEngine::postRender() {
	// If using second pass, draw it to the screen
	if (secondPassShader != nullptr) {
		secondPassShader->drawBufferToScreen();
	}

	// Flush and swap buffers, trigger draw, and poll keypresses
	InputSystem::resetDynamicInputs();
	glfwSwapBuffers(window);
	glfwPollEvents();

	// Reset the collisions flag
	doneCollisionCalc = false;

	timer = GameEngine::engineTimer;
}

void GameEngine::renderNoUpdate() {
	// If we have a skybox, draw it
	if (skyboxShader != nullptr)
		skyboxShader->drawSkybox();

	// Draw
	for (Object* obj : objects) {
		currentObject = obj;
		obj->draw();
	}
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

Object* GameEngine::raycast(glm::vec3 origin, glm::vec3 direction, float distance, float steps, Collider* ignore, bool hitTrigger) {
	direction = glm::normalize(direction);
	float distanceGone = 0.0f;
	float step = distance / steps;

	// Check each step
	while (distanceGone <= distance) {
		glm::vec3 point = origin + (direction * distanceGone);

		// At each step, check each collider (first hit is returned unless matches ignore collider)
		if (hitTrigger) {
			for (Object* obj : triggers) {
				if (obj->getCollider()->pointInCollider(point)) {
					if (ignore != obj->getCollider()) {
						return obj;
					}
				}
			}
		}
		else {
			for (Object* obj : colliders) {
				if (obj->getCollider()->pointInCollider(point)) {
					if (ignore != obj->getCollider()) {
						return obj;
					}
				}
			}
		}

		distanceGone += step;
	}

	return nullptr;
}

void GameEngine::addLight(Light* light, bool forceFirst) {
	if (light->type == LIGHT_TYPE::DIRECTIONAL) {
		if (!forceFirst)
			directionalLights.push_back(light);
		else
			directionalLights.insert(directionalLights.begin(), light);
	}
	else if (light->type == LIGHT_TYPE::AMBIENT) {
		if (!forceFirst)
			ambientLights.push_back(light);
		else
			ambientLights.insert(ambientLights.begin(), light);
	}
	else if (light->type == LIGHT_TYPE::POINT) {
		if (!forceFirst)
			pointLights.push_back(light);
		else
			pointLights.insert(pointLights.begin(), light);
	}
}

void GameEngine::useLights() {
	// Clean up dead lights
	for (long unsigned int i = 0; i < directionalLights.size(); i++) {
		if (directionalLights.at(i)->getDestroy()) {
			Light* obj = directionalLights.at(i);
			directionalLights.erase(directionalLights.begin() + i);
			delete obj;
			i--; // Don't skip over any objects
		}
	}

	for (long unsigned int i = 0; i < ambientLights.size(); i++) {
		if (ambientLights.at(i)->getDestroy()) {
			Light* obj = ambientLights.at(i);
			ambientLights.erase(ambientLights.begin() + i);
			delete obj;
			i--; // Don't skip over any objects
		}
	}

	for (long unsigned int i = 0; i < pointLights.size(); i++) {
		if (pointLights.at(i)->getDestroy()) {
			Light* obj = pointLights.at(i);
			pointLights.erase(pointLights.begin() + i);
			delete obj;
			i--; // Don't skip over any objects
		}
	}

	// Set directional lights
	int directionalToSet = GameEngine::engineShaderProgram->maxDirectionalLights > directionalLights.size() ? GameEngine::engineShaderProgram->maxDirectionalLights : directionalLights.size();
	if (directionalToSet > 0) {
		// Create arrays
		float* r = (float*) malloc(directionalToSet * sizeof(float));
		float* g = (float*) malloc(directionalToSet * sizeof(float));
		float* b = (float*) malloc(directionalToSet * sizeof(float));
		float* intensity = (float*) malloc(directionalToSet * sizeof(float));
		float* x = (float*) malloc(directionalToSet * sizeof(float));
		float* y = (float*) malloc(directionalToSet * sizeof(float));
		float* z = (float*) malloc(directionalToSet * sizeof(float));
		// Put data into arrays
		for (int j = 0; j < directionalToSet; j++) {
			r[j] = directionalLights[j]->r;
			g[j] = directionalLights[j]->g;
			b[j] = directionalLights[j]->b;
			intensity[j] = directionalLights[j]->intensity;
			x[j] = directionalLights[j]->x;
			y[j] = directionalLights[j]->y;
			z[j] = directionalLights[j]->z;
		}
		// Buffer arrays
		GameEngine::engineShaderProgram->uniformFloatArray("directionalR", r, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalG", g, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalB", b, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalIntensity", intensity, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalX", x, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalY", y, directionalToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("directionalZ", z, directionalToSet);
		// Delete arrays
		delete[] r;
		delete[] g;
		delete[] b;
		delete[] intensity;
		delete[] x;
		delete[] y;
		delete[] z;
	}
	GameEngine::engineShaderProgram->uniformInt("directionalCount", directionalToSet);

	// Set ambient lights
	int ambientToSet = GameEngine::engineShaderProgram->maxAmbientLights > ambientLights.size() ? GameEngine::engineShaderProgram->maxAmbientLights : ambientLights.size();
	if (ambientToSet > 0) {
		// Create arrays
		float* r = (float*) malloc(ambientToSet * sizeof(float));
		float* g = (float*) malloc(ambientToSet * sizeof(float));
		float* b = (float*) malloc(ambientToSet * sizeof(float));
		float* intensity = (float*) malloc(ambientToSet * sizeof(float));
		// Put data into arrays
		for (int j = 0; j < ambientToSet; j++) {
			r[j] = ambientLights[j]->r;
			g[j] = ambientLights[j]->g;
			b[j] = ambientLights[j]->b;
			intensity[j] = ambientLights[j]->intensity;
		}
		// Buffer arrays
		GameEngine::engineShaderProgram->uniformFloatArray("ambientR", r, ambientToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("ambientG", g, ambientToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("ambientB", b, ambientToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("ambientIntensity", intensity, ambientToSet);
		// Delete arrays
		delete[] r;
		delete[] g;
		delete[] b;
		delete[] intensity;
	}
	GameEngine::engineShaderProgram->uniformInt("ambientCount", ambientToSet);

	// Set point lights
	int pointToSet = GameEngine::engineShaderProgram->maxPointLights < pointLights.size() ? GameEngine::engineShaderProgram->maxPointLights : pointLights.size();
	if (pointToSet > 0) {
		// Create arrays
		float* r = (float*) malloc(pointToSet * sizeof(float));
		float* g = (float*) malloc(pointToSet * sizeof(float));
		float* b = (float*) malloc(pointToSet * sizeof(float));
		float* intensity = (float*) malloc(pointToSet * sizeof(float));
		float* x = (float*) malloc(pointToSet * sizeof(float));
		float* y = (float*) malloc(pointToSet * sizeof(float));
		float* z = (float*) malloc(pointToSet * sizeof(float));
		// Put data into arrays
		for (int j = 0; j < pointToSet; j++) {
			r[j] = pointLights[j]->r;
			g[j] = pointLights[j]->g;
			b[j] = pointLights[j]->b;
			intensity[j] = pointLights[j]->intensity;
			x[j] = pointLights[j]->x;
			y[j] = pointLights[j]->y;
			z[j] = pointLights[j]->z;
		}
		// Buffer arrays
		GameEngine::engineShaderProgram->uniformFloatArray("pointR", r, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointG", g, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointB", b, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointIntensity", intensity, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointX", x, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointY", y, pointToSet);
		GameEngine::engineShaderProgram->uniformFloatArray("pointZ", z, pointToSet);
		// Delete arrays
		delete[] r;
		delete[] g;
		delete[] b;
		delete[] intensity;
		delete[] x;
		delete[] y;
		delete[] z;
	}
	GameEngine::engineShaderProgram->uniformInt("pointCount", pointToSet);
}

void GameEngine::doCollisionCalc() {
	if (doneCollisionCalc)
		return;

	// Make sure this doesn't destroy the objects
	colliders.erase(colliders.begin(), colliders.begin() + colliders.size());
	triggers.erase(triggers.begin(), triggers.begin() + triggers.size());

	// Go through objects and build up the lists
	for (long unsigned int i = 0; i < objects.size(); i++) {
		if (objects.at(i)->getCollider() == nullptr)
			continue;
		else if (objects.at(i)->getCollider()->isTrigger)
			triggers.push_back(objects.at(i));
		else
			colliders.push_back(objects.at(i));
		objects.at(i)->getCollider()->recalcTransforms();
	}

	doneCollisionCalc = true;
}
