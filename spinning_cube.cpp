
#include "engine/GameEngine.h"

int main () {
	GameEngine *engine = new GameEngine();

	while (engine->render());

	delete engine;

	return 0;
}
