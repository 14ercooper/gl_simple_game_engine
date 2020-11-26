
#ifndef TEXTURE_UTILS
#define TEXTURE_UTILS

#include <GL/glew.h>
#include <stb_image.h>

namespace TextureUtils {
	GLuint loadAndRegisterTexture(const char* filename);
};

#endif
