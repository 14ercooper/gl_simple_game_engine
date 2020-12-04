
#ifndef ENEMY_OBJECT_CLASS
#define ENEMY_OBJECT_CLASS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

#include "engine/Object.h"
#include "engine/shaders/ColorGouradShader.h"
#include "engine/materials/MatteSkyBlue.h"
#include "engine/models/ColoredCube.h"
#include "engine/GameEngine.h"
#include "engine/Collider.h"

// A colored cube object. Default material is matte sky blue.
class EnemyObject : public Object {
public:
	EnemyObject(Object* chase);
	~EnemyObject();

	void draw();

private:
	Object* chase;
	glm::vec2 storedHeading;
};

#endif
