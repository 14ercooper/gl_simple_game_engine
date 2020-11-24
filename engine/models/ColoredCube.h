
#ifndef COLORED_CUBE_CLASS
#define COLORED_CUBE_CLASS

#include <GL/glew.h>

#include "../Model.h"
#include "../Vertex.h"

class ColoredCube : Model {
public:
	void draw();

private:
	static GLuint vao;
	static GLuint vbo;
};

#endif
