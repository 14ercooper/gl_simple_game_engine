
#include "Script.h"

Script::Script() {
	shouldDestroy = false;
}

Script::~Script() {
	// Do nothing
}

void** Script::run(void** args) {
	return (void**) 0;
}

void Script::setDestroy(bool val) {
	shouldDestroy = val;
}

bool Script::getDestroy() {
	return shouldDestroy;
}
