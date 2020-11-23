
#ifndef ENGINE_CLASS
#define ENGINE_CLASS

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GameEngine {
public:
	// Create a new game engine instance. This creates a window for the game.
	GameEngine();
	~GameEngine();

	// Draw the game to the window in it's current state
	// Returns: bool on if the window is still open and being drawn to
	bool render();

private:
	// The window object
	GLFWwindow *window;
};

#endif
