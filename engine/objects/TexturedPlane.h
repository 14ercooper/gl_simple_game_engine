
#ifndef TEX_PLANE_OBJECT_CLASS
#define TEX_PLANE_OBJECT_CLASS

#include <string>

#include "../Object.h"
#include "../shaders/TextureGouradShader.h"
#include "../models/PlaneTextured.h"
#include "../GameEngine.h"
#include "../Collider.h"
#include "../TextureUtils.h"

// A textured plane object. Draws with the given texture.
class TexturedPlaneObject : public Object {
public:
	TexturedPlaneObject(std::string filename);

	void draw();

private:
	GLuint textureHandle;
};

#endif
