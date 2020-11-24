
#include "engine/GameEngine.h"

int main () {
	GameEngine *engine = new GameEngine();

	while (engine->render());

	engine->purgeObjects();
	delete engine;

	return 0;
}
