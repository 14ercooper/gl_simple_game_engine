
#ifndef PLANE_OBJECT_CLASS
#define PLANE_OBJECT_CLASS

#include "../Object.h"
#include "../shaders/ColorGouradShader.h"
#include "../materials/MatteLightGreen.h"
#include "../models/Plane.h"
#include "../GameEngine.h"

class PlaneObject : public Object {
public:
	PlaneObject();

	void draw();
};

#endif
