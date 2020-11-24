
#include "ColoredCube.h"

GLuint ColoredCube::vao = -1;
GLuint ColoredCube::vbo = -1;

void ColoredCube::draw() {
	if (vao == -1) {
		// Set up cube VAO
		// Create vertex array
		Vertex verticies[36] = {
			// Top face
			{-1, 1, -1, 0, 1, 0},
			{-1, 1, 1, 0, 1, 0},
			{1, 1, 1, 0, 1, 0},
			{1, 1, 1, 0, 1, 0},
			{1, 1, -1, 0, 1, 0},
			{-1, 1, -1, 0, 1, 0},

			// Bottom face
			{-1, -1, -1, 0, -1, 0},
			{-1, -1, 1, 0, -1, 0},
			{1, -1, 1, 0, -1, 0},
			{1, -1, 1, 0, -1, 0},
			{1, -1, -1, 0, -1, 0},
			{-1, -1, -1, 0, -1, 0},

			// +X face
			{1, -1, -1, 1, 0 ,0},
			{1, 1, -1, 1, 0 ,0},
			{1, 1, 1, 1, 0 ,0},
			{1, 1, 1, 1, 0 ,0},
			{1, -1, 1, 1, 0 ,0},
			{1, -1, -1, 1, 0 ,0},

			// -X face
			{-1, -1, -1, -1, 0 ,0},
			{-1, 1, -1, -1, 0 ,0},
			{-1, 1, 1, -1, 0 ,0},
			{-1, 1, 1, -1, 0 ,0},
			{-1, -1, 1, -1, 0 ,0},
			{-1, -1, -1, -1, 0 ,0},

			// +Z face
			{-1, -1, 1, 0, 0, 1},
			{-1, 1, 1, 0, 0, 1},
			{1, 1, 1, 0, 0, 1},
			{1, 1, 1, 0, 0, 1},
			{1, -1, 1, 0, 0, 1},
			{-1, -1, 1, 0, 0, 1},

			// -Z face
			{-1, -1, -1, 0, 0, -1},
			{-1, 1, -1, 0, 0, -1},
			{1, 1, -1, 0, 0, -1},
			{1, 1, -1, 0, 0, -1},
			{1, -1, -1, 0, 0, -1},
			{-1, -1, -1, 0, 0, -1},
		};

		// Gen and bind VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Gen and bind VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Enable attribs
		GameEngine::engineShaderProgram->enableAttribs();

		// Buffer data to 
		glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	}

	// Bind VAO
	glBindVertexArray(vao);

	// Draw arrays (GL_TRIANGLES)
	glDrawArrays(GL_TRIANGLES, 0, 36);
}
