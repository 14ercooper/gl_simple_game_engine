
#ifndef PLANE_TEXTURED_MODEL_CLASS
#define PLANE_TEXTURED_MODEL_CLASS

#include <GL/glew.h>

#include "../Model.h"
#include "../Vertex.h"
#include "../GameEngine.h"

// A textured plane model
class PlaneTexturedModel : public Model {
public:
	void draw();

private:
	static GLuint vao;
	static GLuint vbo;
};

#endif
