
#ifndef CUBE_OBJECT_CLASS
#define CUBE_OBJECT_CLASS

#include "../Object.h"
#include "../shaders/ColorPhongShader.h"
#include "../materials/MatteSkyBlue.h"
#include "../models/ColoredCube.h"
#include "../GameEngine.h"

class CubeObject : public Object {
public:
	CubeObject();

	void draw();
};

#endif
