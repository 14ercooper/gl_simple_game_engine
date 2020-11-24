
#include "engine/GameEngine.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"

int main () {
	GameEngine *engine = new GameEngine();
	Quaternion *rotateCube = new Quaternion();
	rotateCube->euler(0.05f, 1, 2, 1);

	CubeObject *cube = new CubeObject();

	engine->addObject(cube);

	while (engine->render()) {
		cube->rotation->hamilton(rotateCube);
	}

	engine->purgeObjects();
	delete engine;

	return 0;
}
