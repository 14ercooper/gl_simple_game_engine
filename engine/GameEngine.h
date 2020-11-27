
#ifndef ENGINE_CLASS
#define ENGINE_CLASS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <cstdio>

#include "Object.h"
#include "Collider.h"
#include "ShaderProgram.h"
#include "Camera.h"
#include "shaders/SecondPassShader.h"
#include "shaders/SkyboxShader.h"
#include "InputSystem.h"
#include "Script.h"

class GameEngine {
public:
	// Create a new game engine instance. This creates a window for the game.
	GameEngine();
	~GameEngine();

	// Set the window size. If either are < 1, full screen
	void setWindowSize(int width, int height);

	// Empty objects
	void purgeObjects();

	// Set a second pass shader or skybox shader
	void setSecondPass(SecondPassShader* shader, bool destroyOld);
	void setSkyboxShader(SkyboxShader* shader, bool destroyOld);

	// Set the camera
	void setCamera(Camera* cam, bool destroyOld);

	// Draw the game to the window in it's current state
	// Returns: bool on if the window is still open and being drawn to
	bool render();

	// Add an object or script to the game
	void addObject(Object* o);
	void addScript(Script* s);

	// Check for collisions
	std::vector<Object*> checkCollisions(Collider* c);
	std::vector<Object*> checkTriggers(Collider* c);
	std::vector<Object*> checkCollisions(Collider* c, float testX, float testY, float testZ);
	std::vector<Object*> checkTriggers(Collider* c, float testX, float testY, float testZ);

	// Statics to be accessed and used in objects
	static GameEngine* engine;
	static GLFWwindow* engineWindow;
	static Camera* engineCamera;
	static ShaderProgram* engineShaderProgram;
	static Object* currentObject;

private:
	// The window object
	GLFWwindow *window;

	// World objects
	std::vector<Object*> objects;

	// Collision map is only calculated one per frame, before physics
	bool doneCollisionCalc;
	void doCollisionCalc();
	std::vector<Object*> colliders;
	std::vector<Object*> triggers;

	// Camera
	Camera* camera;

	// Second pass shader and skybox shader
	SecondPassShader* secondPassShader;
	SkyboxShader* skyboxShader;

	// Engine scripts
	std::vector<Script*> engineScripts;
};

#endif
