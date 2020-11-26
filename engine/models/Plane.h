
#ifndef PLANE_MODEL_CLASS
#define PLANE_MODEL_CLASS

#include <GL/glew.h>

#include "../Model.h"
#include "../Vertex.h"
#include "../GameEngine.h"

class PlaneModel : public Model {
public:
	void draw();

private:
	static GLuint vao;
	static GLuint vbo;
};

#endif
