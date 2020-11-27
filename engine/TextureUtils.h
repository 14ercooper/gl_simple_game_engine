
#ifndef TEXTURE_UTILS
#define TEXTURE_UTILS

#include <GL/glew.h>
#include <stb_image.h>

// Useful for handling textures
namespace TextureUtils {
	// Load and register a texture by filename
	// Returns the texture handle
	GLuint loadAndRegisterTexture(const char* filename);
}

#endif
