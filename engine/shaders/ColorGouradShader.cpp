
#include "ColorGouradShader.h"

const char* ColorGouradShader::vertexProgram =
R"FOURTEENER(#version 410 core

in vec3 vertexPosition;
in vec3 vertexNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

uniform vec3 materialColor;
uniform float materialDiffuse;
uniform float materialSpecularity;

uniform vec3 viewingVector;

// Lights
uniform int directionalCount;
uniform float directionalR[1];
uniform float directionalG[1];
uniform float directionalB[1];
uniform float directionalIntensity[1];
uniform float directionalX[1];
uniform float directionalY[1];
uniform float directionalZ[1];

uniform int ambientCount;
uniform float ambientR[1];
uniform float ambientG[1];
uniform float ambientB[1];
uniform float ambientIntensity[1];

uniform int pointCount;
uniform float pointR[8];
uniform float pointG[8];
uniform float pointB[8];
uniform float pointIntensity[8];
uniform float pointX[8];
uniform float pointY[8];
uniform float pointZ[8];

out vec3 color;

float distSquared (vec3 a, vec3 b) {
	vec3 c = a - b;
	return dot(c, c);
}

void main() {
	vec4 transformedVertexPos = modelMatrix * vec4(vertexPosition, 1);
	gl_Position = projectionMatrix * viewMatrix * transformedVertexPos;
	mat3 normalMatrix = mat3(transpose(inverse(modelMatrix)));
	vec3 normalVector = normalize(normalMatrix * vertexNormal);
	vec3 normalViewingVector = normalize(viewingVector);

	// Coloring
	vec3 diffuseColor = vec3(0, 0, 0);
	vec3 specularColor = vec3(0, 0, 0);
	vec3 ambientColor = vec3(0, 0, 0);
	float alpha = 2;

	// Directional
	for (int i = 0; i < directionalCount; i++) {
		vec3 sunDirection = normalize(vec3(directionalX[i], directionalY[i], directionalZ[i]));
		vec3 sunColor = vec3(directionalR[i], directionalG[i], directionalB[i]);
		vec3 color = materialColor * sunColor;
		color *= directionalIntensity[i];

		// Calc diffuse
		float diffuseIntensity = dot(sunDirection, normalVector);
		vec3 colorIntensity = color * clamp(diffuseIntensity, 0, 1);
		diffuseColor += colorIntensity;

		// Calc specular
		vec3 reflectionVector = -sunDirection + (2 * dot(normalVector, sunDirection)) * normalVector;
		vec3 specularIntensity = color * clamp(pow(max(dot(normalViewingVector, reflectionVector), 0), alpha), 0, 1);
		specularColor += specularIntensity;
	}

	// Point
	for (int i = 0; i < pointCount; i++) {
		vec3 lightPos = vec3(pointX[i], pointY[i], pointZ[i]);
		vec3 lightDirection = normalize(lightPos - vec3(transformedVertexPos));
		vec3 lightColor = vec3(pointR[i], pointG[i], pointB[i]);
		float distance = distSquared(lightPos, vec3(transformedVertexPos));
		float intensity = pointIntensity[i] * (1.0 / (1.0 + (0.25 * distance)));
		vec3 color = materialColor * lightColor;
		color = intensity * color;

		// Calc diffuse
		float diffuseIntensity = dot(lightDirection, normalVector);
		vec3 colorIntensity = color * clamp(diffuseIntensity, 0, 1);
		diffuseColor += colorIntensity;

		// Calc specular
		vec3 reflectionVector = -lightDirection + (2 * dot(normalVector, lightDirection)) * normalVector;
		vec3 specularIntensity = color * clamp(pow(max(dot(normalViewingVector, reflectionVector), 0), alpha), 0, 1);
		specularColor += specularIntensity;
	}

	// Ambient light
	for (int i = 0; i < ambientCount; i++) {
		ambientColor += ambientIntensity[i] * vec3(ambientR[i], ambientG[i], ambientB[i]) * materialColor;
	}

	// Combine with weights
	color = (materialDiffuse * diffuseColor) + (materialSpecularity * specularColor) + ambientColor;
}
)FOURTEENER";

const char* ColorGouradShader::fragmentProgram =
R"FOURTEENER(#version 410 core

in vec3 color;
out vec4 fragColorOut;

void main() {
	fragColorOut = vec4(color, 1);
}
)FOURTEENER";

ColorGouradShader::ColorGouradShader() {
	maxDirectionalLights = 1;
	maxAmbientLights = 1;
	maxPointLights = 8;

	GLuint vertex = compileShaderText(ColorGouradShader::vertexProgram, GL_VERTEX_SHADER);
	GLuint fragment = compileShaderText(ColorGouradShader::fragmentProgram, GL_FRAGMENT_SHADER);

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

void ColorGouradShader::enableAttribs() {
	useProgram();

	// Enable attributes
	GLuint vertexPosAttr = getAttributeLocation("vertexPosition");
	GLuint vertexNormalAttr = getAttributeLocation("vertexNormal");

	glEnableVertexAttribArray(vertexPosAttr);
	glVertexAttribPointer(vertexPosAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) 0);

	glEnableVertexAttribArray(vertexNormalAttr);
	glVertexAttribPointer(vertexNormalAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
}

