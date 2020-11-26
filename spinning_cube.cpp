
#include <glm/glm.hpp>

#include <cstdio>

#include "engine/GameEngine.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"
#include "engine/objects/Plane.h"
#include "engine/scripts/SimplePhysics.h"

int main () {
	GameEngine *engine = new GameEngine();
	engine->setWindowSize(-1, -1);
	
	Quaternion *rotateCube = new Quaternion();
	rotateCube->euler(0.01f, 1, 2, 1);

	CubeObject *cube = new CubeObject();
	cube->translate(glm::vec3(-2.0f, 3.0f, 0.0f));
	Script* physics = new SimplePhysics();
	cube->setPhysicsTick(physics, true);
	engine->addObject(cube);

	PlaneObject *ground = new PlaneObject();
	ground->translate(glm::vec3(0.0f, -3.0f, 0.0f));
	ground->scale(glm::vec3(10.0f, 1.0f, 10.0f));
	engine->addObject(ground);

	while (engine->render()) {
		cube->rotation->hamilton(rotateCube);
	}

	delete rotateCube;

	engine->purgeObjects();
	delete engine;

	return 0;
}
