
#include "PlaneTextured.h"

GLuint PlaneTexturedModel::vao = -1;
GLuint PlaneTexturedModel::vbo = -1;

void PlaneTexturedModel::draw() {
	if (vao == (GLuint) -1) {
		VertexTextured verticies[6] = { // TODO figure out why these normals need to be negative
			{-1, 0, -1, 0, -1, 0, 0, 0},
			{-1, 0, 1, 0, -1, 0, 0, 1},
			{1, 0, 1, 0, -1, 0, 1, 1},
			{1, 0, 1, 0, -1, 0, 1, 1},
			{1, 0, -1, 0, -1, 0, 1, 0},
			{-1, 0, -1, 0, -1, 0, 0, 0},
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
