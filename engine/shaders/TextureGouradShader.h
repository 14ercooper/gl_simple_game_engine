
#ifndef TEXTUTRE_GOURAD_SHADER_CLASS
#define TEXTUTRE_GOURAD_SHADER_CLASS

#include <GL/glew.h>
#include <string>

#include "../ShaderProgram.h"
#include "../GameEngine.h"

class TextureGouradShader : public ShaderProgram {
public:
	TextureGouradShader();

	void enableAttribs();

	static const char* vertexProgram;
	static const char* fragmentProgram;
};

#endif
