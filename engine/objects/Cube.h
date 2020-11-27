
#ifndef CUBE_OBJECT_CLASS
#define CUBE_OBJECT_CLASS

#include "../Object.h"
#include "../shaders/ColorGouradShader.h"
#include "../materials/MatteSkyBlue.h"
#include "../models/ColoredCube.h"
#include "../GameEngine.h"
#include "../Collider.h"

// A colored cube object. Default material is matte sky blue.
class CubeObject : public Object {
public:
	CubeObject();
	~CubeObject();

	void draw();
};

#endif
