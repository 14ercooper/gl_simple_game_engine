#version 410 core

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
