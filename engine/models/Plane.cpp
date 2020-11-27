
#include "Plane.h"

GLuint PlaneModel::vao = -1;
GLuint PlaneModel::vbo = -1;

void PlaneModel::draw() {
	if (vao == (GLuint) -1) {
		Vertex verticies[6] = { // TODO figure out why these normals need to be negative
			{-1, 0, -1, 0, -1, 0},
			{-1, 0, 1, 0, -1, 0},
			{1, 0, 1, 0, -1, 0},
			{1, 0, 1, 0, -1, 0},
			{1, 0, -1, 0, -1, 0},
			{-1, 0, -1, 0, -1, 0},
		};

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		GameEngine::engineShaderProgram->enableAttribs();

		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
