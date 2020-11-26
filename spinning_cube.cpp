
#include <glm/glm.hpp>

#include "engine/GameEngine.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"
#include "engine/objects/Plane.h"

int main () {
	GameEngine *engine = new GameEngine();
	
	Quaternion *rotateCube = new Quaternion();
	rotateCube->euler(0.01f, 1, 2, 1);

	Quaternion *rotateCube2 = new Quaternion();
	rotateCube2->euler(0.02f, -1, 2, 1);

	Quaternion *rotateCube3 = new Quaternion();
	rotateCube3->euler(0.01f, 1, 2, -1);

	CubeObject *cube = new CubeObject();
	engine->addObject(cube);

	CubeObject *cube2 = new CubeObject();
	cube2->translate(glm::vec3(-3.0f, 0.0f, -2.0f));
	engine->addObject(cube2);
	
	CubeObject *cube3 = new CubeObject();
	cube3->translate(glm::vec3(-3.0f, 0.0f, 2.0f));
	engine->addObject(cube3);

	PlaneObject *ground = new PlaneObject();
	ground->translate(glm::vec3(0.0f, -3.0f, 0.0f));
	ground->scale(glm::vec3(10.0f, 0.001f, 10.0f));
	engine->addObject(ground);

	while (engine->render()) {
		cube->rotation->hamilton(rotateCube);
		cube2->rotation->hamilton(rotateCube2);
		cube3->rotation->hamilton(rotateCube3);
	}

	delete rotateCube;
	delete rotateCube2;
	delete rotateCube3;

	engine->purgeObjects();
	delete engine;

	return 0;
}
