
#include "Cube.h"

CubeObject::CubeObject () {
	// Load shader
	shaderProgram = new ColorPhongShader();

	// Load material
	material = new MatteSkyBlue();

	// Load model
	model = new ColoredCube();
}

void CubeObject::draw() {
	// Use shader
	shaderProgram->useProgram();

	// Buffer mvp uniforms
	shaderProgram->uniformMat4("modelMatrix", getModelMatrix());
	shaderProgram->uniformMat4("viewMatrix", GameEngine::engineCamera->getViewMatrix());
	shaderProgram->uniformMat4("projectionMatrix", GameEngine::engineCamera->getProjectionMatrix(GameEngine::engineWindow));

	// Buffer material uniforms
	shaderProgram->uniformVec3("materialColor", glm::vec3(material->r, material->g, material->b));
	shaderProgram->uniformFloat("materialDiffuse", material->diffuse);
	shaderProgram->uniformFloat("materialSpecularity", material->specular);

	// Buffer viewing vector uniform
	shaderProgram->uniformVec3("viewingVector", GameEngine::engineCamera->getViewingVector());

	// Draw model
}
