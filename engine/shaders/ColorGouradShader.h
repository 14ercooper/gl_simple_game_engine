
#ifndef COLOR_GOURAD_SHADER_CLASS
#define COLOR_GOURAD_SHADER_CLASS

#include <GL/glew.h>
#include <string>

#include "../ShaderProgram.h"
#include "../GameEngine.h"

class ColorGouradShader : public ShaderProgram {
public:
	ColorGouradShader();

	void enableAttribs();

	static const char* vertexProgram;
	static const char* fragmentProgram;
};

#endif
