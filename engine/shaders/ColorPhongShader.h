
#ifndef COLOR_PHONG_SHADER_CLASS
#define COLOR_PHONG_SHADER_CLASS

#include <GL/glew.h>

#include "../ShaderProgram.h"
#include "../GameEngine.h"

class ColorPhongShader : public ShaderProgram {
public:
	ColorPhongShader();

	void enableAttribs();
};

#endif
