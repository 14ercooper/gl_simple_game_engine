
#include "Plane.h"

PlaneObject::PlaneObject () {
	shaderProgram = new ColorGouradShader();
	material = new MatteLightGreen();
	model = new PlaneModel();
	setCollider(new Collider(this), false);
	scale(glm::vec3(1.0f, 0.1f, 1.0f));
}

void PlaneObject::draw() {
	shaderProgram->useProgram();
	GameEngine::engineShaderProgram = shaderProgram;

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

	material->use();

	// Draw model
	model->draw();
}
