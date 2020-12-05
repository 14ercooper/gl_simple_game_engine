
#ifndef SECOND_PASS_SHADER
#define SECOND_PASS_SHADER

#include <GL/glew.h>

#include "../ShaderProgram.h"

// A passthrough second pass shader, able to be extended to implement other functionalities on a second pass
class SecondPassShader : public ShaderProgram {
public:
	SecondPassShader(int fboWidth, int fboHeight);
	SecondPassShader();
	~SecondPassShader();

	void enableAttribs();

	void setDrawToBuffer();
	void drawBufferToScreen();

	static const char* passthroughVertexProgram;
	static const char* passthroughFragmentProgram;

protected:
	int width, height;
	GLuint fbo, rbo, textureHandle;
	GLuint quadVAO, quadVBO, quadIBO;
};

#include "../GameEngine.h"

#endif
