
#ifndef COLOR_PHONG_SHADER_CLASS
#define COLOR_PHONG_SHADER_CLASS

#include <GL/glew.h>
#include <string>

#include "../ShaderProgram.h"
#include "../GameEngine.h"

class ColorPhongShader : public ShaderProgram {
public:
	ColorPhongShader();

	void enableAttribs();

	static const char* vertexProgram;
	static const char* fragmentProgram;
};

#endif
