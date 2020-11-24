
#include "ColorPhongShader.h"

ColorPhongShader::ColorPhongShader() {
	GLuint vertex = compileShader("engine/shaders/glsl/colorPhong.v.glsl", GL_VERTEX_SHADER);
	GLuint fragment = compileShader("engine/shaders/glsl/colorPhong.f.glsl", GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	glAttachShader(programHandle, vertex);
	glAttachShader(programHandle, fragment);

	glLinkProgram(programHandle);

	printLog(programHandle);

	glDetachShader(programHandle, vertex);
	glDetachShader(programHandle, fragment);

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glUseProgram(programHandle);

	// Enable attributes
	GLuint vertexPosAttr = getAttributeLocation("vertexPosition");
	GLuint vertexNormalAttr = getAttributeLocation("vertexAttribute");

	glEnableVertexAttribArray(vertexPosAttr);
	glVertexAttribPointer(vertexPosAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);

	glEnableVertexAttribArray(vertexNormalAttr);
	glVertexAttribPointer(vertexNormalAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
}

