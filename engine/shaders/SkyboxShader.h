
#ifndef SKYBOX_SHADER
#define SKYBOX_SHADER

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <stb_image.h>

#include "../ShaderProgram.h"

class SkyboxShader : public ShaderProgram {
public:
	// Order: px nx py ny pz nz
	SkyboxShader(std::vector<std::string> textures);

	void enableAttribs();

	void drawSkybox();

	static const char* vertexProgram;
	static const char* fragmentProgram;

private:
	GLuint cubemapId, vao, vbo;
};

#include "../GameEngine.h"

#endif
