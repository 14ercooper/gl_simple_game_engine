
#ifndef PARTICLE_SHADER
#define PARTICLE_SHADER

#include <string>
#include <GL/glew.h>

#include "../ShaderProgram.h"
#include "../TextureUtils.h"

// A billboarding particle shader with transparency support
class ParticleShader : public ShaderProgram {
public:
	ParticleShader(std::string texture);

	void enableAttribs();

	void drawParticle(float x, float y, float z, float size);

	static const char* vertexProgram;
	static const char* geometryProgram;
	static const char* fragmentProgram;

private:
	GLuint vao, vbo, textureId;
};

#endif
