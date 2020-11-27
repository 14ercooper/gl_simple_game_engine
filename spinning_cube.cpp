
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <cstdio>

#include "engine/GameEngine.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"
#include "engine/objects/TexturedPlane.h"
#include "engine/objects/ParticleSystem.h"
#include "engine/scripts/SimplePhysics.h"
#include "engine/scripts/CloseOnEscape.h"
#include "engine/shaders/SecondPassShader.h"
#include "engine/shaders/SkyboxShader.h"

// Get rand [-1..1]
double getRand() { return (2.0*(rand() / (double)RAND_MAX)) - 1; }

int main () {
	// Create a new game engine and full screen the window
	GameEngine *engine = new GameEngine();
	engine->setWindowSize(-1, -1);

	// Add a skybox
	std::vector<std::string> skyTextures = {"textures/skybox/px.png",
			"textures/skybox/nx.png", "textures/skybox/py.png",
			"textures/skybox/ny.png", "textures/skybox/pz.png",
			"textures/skybox/nz.png"};
	SkyboxShader* skybox = new SkyboxShader(skyTextures);
	engine->setSkyboxShader(skybox, true);

	// Make the window close when we press escape
	CloseOnEscape* closeScript = new CloseOnEscape();
	engine->addScript(closeScript);
	
	// This can be used to rotate the cube
	Quaternion *rotateCube = new Quaternion();
	rotateCube->euler(0.01f, 1, 2, 1);

	// Add 3 cube objects
	CubeObject *cube = new CubeObject();
	cube->translate(glm::vec3(0.0f, 20.0f, 0.0f));
	Script* physics = new SimplePhysics();
	cube->setPhysicsTick(physics, true);
	engine->addObject(cube);

	CubeObject *cube2 = new CubeObject();
	cube2->translate(glm::vec3(0.0f, 20.0f, -5.0f));
	cube2->setPhysicsTick(physics, true);
	engine->addObject(cube2);

	CubeObject *cube3 = new CubeObject();
	cube3->translate(glm::vec3(0.0f, 20.0f, 5.0f));
	cube3->setPhysicsTick(physics, true);
	engine->addObject(cube3);

	// Create a particle system
	ParticleSystem* particles = new ParticleSystem("textures/particle.png", 0.0f);
	engine->addObject(particles);

	// Turn on 2 pass rendering
	SecondPassShader* secondPass = new SecondPassShader(1920, 1080);
	engine->setSecondPass(secondPass, true);

	// Add a ground
	TexturedPlaneObject *ground = new TexturedPlaneObject("textures/ground.png");
	ground->translate(glm::vec3(0.0f, -3.0f, 0.0f));
	ground->scale(glm::vec3(10.0f, 1.0f, 10.0f));

	Quaternion* groundRot = new Quaternion();
	groundRot->euler(-0.5f, 1, 0, 0);
	ground->rotation->hamilton(groundRot);
	ground->getCollider()->trackParentRotation = true;
	ground->getCollider()->recalcTransforms();
	delete groundRot;

	engine->addObject(ground);

	// Render
	while (engine->render()) {
		// Apply the rotation to the cube
		cube->rotation->hamilton(rotateCube);
		cube2->rotation->hamilton(rotateCube);
		cube3->rotation->hamilton(rotateCube);

		// Spawn particles
		if (particles->size() < 10)
			if (getRand() < -0.75)
				particles->addParticle(10 * getRand(), 10 * getRand(), 10 * getRand(),
					0.05 * getRand(), 0.05 * getRand(), 0.05 * getRand(), 200);
	}

	// Clean up the engine
	engine->purgeObjects();
	delete engine;

	// Clean up the other stuff
	delete secondPass;
	delete skybox;
	delete rotateCube;
	delete closeScript;

	// Return successfully
	return 0;
}
