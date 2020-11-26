
#ifndef SECOND_PASS_SHADER
#define SECOND_PASS_SHADER

#include <GL/glew.h>

#include "../ShaderProgram.h"

class SecondPassShader : public ShaderProgram {
public:
	SecondPassShader(int fboWidth, int fboHeight);

	void enableAttribs();

	void setDrawToBuffer();
	void drawBufferToScreen();

	static const char* passthroughVertexProgram;
	static const char* passthroughFragmentProgram;

private:
	int width, height;
	GLuint fbo, rbo, textureHandle;
	GLuint quadVAO, quadVBO, quadIBO;
};

#include "../GameEngine.h"

#endif
