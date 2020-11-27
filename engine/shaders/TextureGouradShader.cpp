
#include "TextureGouradShader.h"

const char* TextureGouradShader::vertexProgram =
R"FOURTEENER(#version 410 core

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexture;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform float materialDiffuse;
uniform float materialSpecularity;

uniform vec3 viewingVector;

out vec2 texCoord;
out float diffuseIntensity;
out float specularIntensity;
out float ambientIntensity;

void main() {
	vec4 transformedVertexPos = modelMatrix * vec4(vertexPosition, 1);
	gl_Position = projectionMatrix * viewMatrix * transformedVertexPos;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	vec3 normalVector = normalize(normalMatrix * vertexNormal);
	vec3 normalViewingVector = normalize(viewingVector);

	// Global light (change later)
	vec3 sunDirection = normalize(vec3(1, 1, -1));

	// Coloring
	float alpha = 2;

	// Calc 
	diffuseIntensity = clamp(dot(sunDirection, normalVector), 0, 1) * materialDiffuse;

	// Calc specular
	vec3 reflectionVector = -sunDirection + (2 * dot(normalVector, sunDirection)) * normalVector;
	specularIntensity = clamp(pow(max(dot(normalViewingVector, reflectionVector), 0), alpha), 0, 1) * materialSpecularity;

	// Ambient light
	ambientIntensity = 0.05;

	// Texture
	texCoord = vertexTexture;
}
)FOURTEENER";

const char* TextureGouradShader::fragmentProgram =
R"FOURTEENER(#version 410 core

uniform sampler2D tex;

in vec2 texCoord;
in float diffuseIntensity;
in float specularIntensity;
in float ambientIntensity;

out vec4 fragColorOut;

void main() {
	vec4 texel = texture(tex, texCoord);
	float texA = texel.a;

	texel = (texel * diffuseIntensity) + (texel * specularIntensity) + (texel * ambientIntensity);

	texel.a = texA;

	fragColorOut = texel;
}
)FOURTEENER";

TextureGouradShader::TextureGouradShader() {
	GLuint vertex = compileShaderText(TextureGouradShader::vertexProgram, GL_VERTEX_SHADER);
	GLuint fragment = compileShaderText(TextureGouradShader::fragmentProgram, GL_FRAGMENT_SHADER);

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

	glUniform1i(getUniformLocation("tex"), 0);
}

void TextureGouradShader::enableAttribs() {
	useProgram();

	// Enable attributes
	GLuint vertexPosAttr = getAttributeLocation("vertexPosition");
	GLuint vertexNormalAttr = getAttributeLocation("vertexNormal");
	GLuint vertexTextureAttr = getAttributeLocation("vertexTexture");

	glEnableVertexAttribArray(vertexPosAttr);
	glVertexAttribPointer(vertexPosAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) 0);

	glEnableVertexAttribArray(vertexNormalAttr);
	glVertexAttribPointer(vertexNormalAttr, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(vertexTextureAttr);
	glVertexAttribPointer(vertexTextureAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*) (6 * sizeof(GLfloat)));
}
