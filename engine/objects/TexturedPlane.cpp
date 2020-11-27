
#include "TexturedPlane.h"

TexturedPlaneObject::TexturedPlaneObject(std::string filename) {
	shaderProgram = new TextureGouradShader();
	model = new PlaneTexturedModel();
	setCollider(new Collider(this), false);
	scale(glm::vec3(1.0f, 0.0001f, 1.0f));
	textureHandle = TextureUtils::loadAndRegisterTexture(filename.c_str());
}

void TexturedPlaneObject::draw() {
	shaderProgram->useProgram();
	GameEngine::engineShaderProgram = shaderProgram;

	// Buffer mvp uniforms and view vector
	shaderProgram->uniformMat4("modelMatrix", getModelMatrix());
	shaderProgram->uniformMat4("viewMatrix", GameEngine::engineCamera->getViewMatrix());
	shaderProgram->uniformMat4("projectionMatrix", GameEngine::engineCamera->getProjectionMatrix(GameEngine::engineWindow));
	shaderProgram->uniformVec3("viewingVector", GameEngine::engineCamera->getViewingVector());

	// Material diffuse and spec
	shaderProgram->uniformFloat("materialDiffuse", 1.0f);
	shaderProgram->uniformFloat("materialSpecularity", 0.05f);

	// Bind texture
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	// Draw
	material->use();
	model->draw();
}
