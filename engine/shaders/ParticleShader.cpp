
#include "ParticleShader.h"

const char* ParticleShader::vertexProgram =
R"FOURTEENER(#version 410 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in float size;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out float sizeOut;

void main() {
	sizeOut = size;
	gl_Position = viewMatrix * modelMatrix * vec4(vPos, 1);
}
)FOURTEENER";

const char* ParticleShader::geometryProgram =
R"FOURTEENER(#version 410 core

layout(points) in;
uniform mat4 projectionMatrix;
in float sizeOut[];

layout(triangle_strip, max_vertices=4) out;
out vec2 texCoord;

void main() {
	float size = sizeOut[0];

	gl_Position = projectionMatrix * (vec4(gl_in[0].gl_Position.xyz, 1) + vec4(-1 * size, -1 * size, 0, 0));
    texCoord = vec2(0, 1);
    EmitVertex();

    gl_Position = projectionMatrix * (vec4(gl_in[0].gl_Position.xyz, 1) + vec4(-1 * size, 1 * size, 0, 0));
    texCoord = vec2(0, 0);
    EmitVertex();

	gl_Position = projectionMatrix * (vec4(gl_in[0].gl_Position.xyz, 1) + vec4(1 * size, -1 * size, 0, 0));
    texCoord = vec2(1, 1);
    EmitVertex();

    gl_Position = projectionMatrix * (vec4(gl_in[0].gl_Position.xyz, 1) + vec4(1 * size, 1 * size, 0, 0));
    texCoord = vec2(1, 0);
    EmitVertex();

    EndPrimitive();
}
)FOURTEENER";

const char* ParticleShader::fragmentProgram =
R"FOURTEENER(#version 410 core

in vec2 texCoord;
uniform sampler2D tex;

out vec4 fragColorOut;

void main() {
	fragColorOut = texture(tex, texCoord);

	if (fragColorOut.a < 0.001) {
		discard;
	}
}
)FOURTEENER";

ParticleShader::ParticleShader(std::string texture) {
	// Load shader
	GLuint vertex = compileShaderText(ParticleShader::vertexProgram, GL_VERTEX_SHADER);
	GLuint geometry = compileShaderText(ParticleShader::geometryProgram, GL_GEOMETRY_SHADER);
	GLuint fragment = compileShaderText(ParticleShader::fragmentProgram, GL_FRAGMENT_SHADER);

	programHandle = glCreateProgram();

	glAttachShader(programHandle, vertex);
	glAttachShader(programHandle, geometry);
	glAttachShader(programHandle, fragment);

	glLinkProgram(programHandle);

	printLog(programHandle);

	glDetachShader(programHandle, vertex);
	glDetachShader(programHandle, geometry);
	glDetachShader(programHandle, fragment);

	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);

	glUseProgram(programHandle);

	// Load texture
	textureId = TextureUtils::loadAndRegisterTexture(texture.c_str());

	// Set up buffers
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

	// Enable attribs and such
	this->enableAttribs();
	glUniform1i(getUniformLocation("tex"), 0);
}

void ParticleShader::enableAttribs() {
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (3 * sizeof(GLfloat)));
}

void ParticleShader::drawParticle(float x, float y, float z, float size) {
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	float data[] = {x, y, z, size};
	glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(GLfloat), data);
	glDrawArrays(GL_POINTS, 0, 1);
}
