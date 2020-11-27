
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <cstdio>

#include "engine/GameEngine.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"
#include "engine/objects/TexturedPlane.h"
#include "engine/scripts/SimplePhysics.h"
#include "engine/scripts/CloseOnEscape.h"
#include "engine/shaders/SecondPassShader.h"
#include "engine/shaders/SkyboxShader.h"

int main () {
	GameEngine *engine = new GameEngine();
	engine->setWindowSize(-1, -1);

	std::vector<std::string> skyTextures = {"textures/skybox/px.png",
			"textures/skybox/nx.png", "textures/skybox/py.png",
			"textures/skybox/ny.png", "textures/skybox/pz.png",
			"textures/skybox/nz.png"};
	SkyboxShader* skybox = new SkyboxShader(skyTextures);
	engine->setSkyboxShader(skybox, true);

	CloseOnEscape* closeScript = new CloseOnEscape();
	engine->addScript(closeScript);
	
	Quaternion *rotateCube = new Quaternion();
	rotateCube->euler(0.01f, 1, 2, 1);

	CubeObject *cube = new CubeObject();
	cube->translate(glm::vec3(-2.0f, 3.0f, 0.0f));
	Script* physics = new SimplePhysics();
	cube->setPhysicsTick(physics, true);
	engine->addObject(cube);

	SecondPassShader* secondPass = new SecondPassShader(1920, 1080);
	engine->setSecondPass(secondPass, true);

	TexturedPlaneObject *ground = new TexturedPlaneObject("textures/ground.png");
	ground->translate(glm::vec3(0.0f, -3.0f, 0.0f));
	ground->scale(glm::vec3(10.0f, 1.0f, 10.0f));
	engine->addObject(ground);

	while (engine->render()) {
		cube->rotation->hamilton(rotateCube);
	}

	engine->purgeObjects();
	delete engine;

	delete secondPass;
	delete skybox;
	delete rotateCube;
	delete closeScript;

	return 0;
}
