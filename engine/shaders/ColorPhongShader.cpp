
#include "ColorPhongShader.h"

const char* ColorPhongShader::vertexProgram =
R"V0G0N(#version 410 core

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 materialColor;
uniform float materialDiffuse;
uniform float materialSpecularity;

uniform vec3 viewingVector;

out vec3 color;

void main() {
	vec4 transformedVertexPos = modelMatrix * vec4(vertexPosition, 1);
	gl_Position = projectionMatrix * viewMatrix * transformedVertexPos;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	vec3 normalVector = normalize(normalMatrix * vertexNormal);
	vec3 normalViewingVector = normalize(viewingVector);

	// Global light (change later)
	vec3 sunDirection = normalize(vec3(1, 1, -1));

	// Coloring
	vec3 diffuseColor = vec3(0, 0, 0);
	vec3 specularColor = vec3(0, 0, 0);
	float alpha = 2;

	// Calc 
	float diffuseIntensity = dot(sunDirection, normalVector);
	vec3 colorIntensity = materialColor * max(diffuseIntensity, 0);
	diffuseColor = diffuseColor + colorIntensity;

	// Calc specular
	vec3 reflectionVector = -sunDirection + (2 * dot(normalVector, sunDirection)) * normalVector;
	vec3 specularIntensity = materialColor * pow(max(dot(normalViewingVector, reflectionVector), 0), alpha);
	specularColor = specularColor + specularIntensity;

	// Ambient light
	vec3 ambientColor = vec3(0.05, 0.05, 0.05);

	// Combine with weights
	color = (materialDiffuse * diffuseColor) + (materialSpecularity * specularColor) + ambientColor;
}
)V0G0N";

const char* ColorPhongShader::fragmentProgram =
R"V0G0N(#version 410 core

in vec3 color;
out vec4 fragColorOut;

void main() {
	fragColorOut = vec4(color, 1);
}
)V0G0N";

ColorPhongShader::ColorPhongShader() {
	GLuint vertex = compileShaderText(ColorPhongShader::vertexProgram, GL_VERTEX_SHADER);
	GLuint fragment = compileShaderText(ColorPhongShader::fragmentProgram, GL_FRAGMENT_SHADER);

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
}

void ColorPhongShader::enableAttribs() {
	// Enable attributes
	GLuint vertexPosAttr = getAttributeLocation("vertexPosition");
	GLuint vertexNormalAttr = getAttributeLocation("vertexNormal");

	glEnableVertexAttribArray(vertexPosAttr);
	glVertexAttribPointer(vertexPosAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);

	glEnableVertexAttribArray(vertexNormalAttr);
	glVertexAttribPointer(vertexNormalAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
}

