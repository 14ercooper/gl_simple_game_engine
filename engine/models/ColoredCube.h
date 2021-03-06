
#ifndef COLORED_CUBE_CLASS
#define COLORED_CUBE_CLASS

#include <GL/glew.h>

#include "../Model.h"
#include "../Vertex.h"
#include "../GameEngine.h"

// A cube that can be colored with a material
class ColoredCube : public Model {
public:
	void draw();

private:
	static GLuint vao;
	static GLuint vbo;
};

#endif
