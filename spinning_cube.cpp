
#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <cstdio>
#include <ctime>

#include "engine/GameEngine.h"
#include "engine/Camera.h"
#include "engine/math/Quaternion.h"
#include "engine/objects/Cube.h"
#include "engine/objects/TexturedPlane.h"
#include "engine/objects/ParticleSystem.h"
#include "engine/scripts/CloseOnEscape.h"
#include "engine/scripts/SimpleMovement.cpp"
#include "engine/shaders/SecondPassShader.h"
#include "engine/shaders/SkyboxShader.h"
#include "engine/materials/MetallicOrange.h"

#include "Enemy.h"
#include "OutOfBoundsScript.h"
#include "BouncePhysics.h"

// Get rand [-1..1]
double getRand() { return (2.0*(rand() / (double)RAND_MAX)) - 1; }

int main () {
	srand(time(0));
	rand(); rand();

	// Create a new game engine and full screen the window
	GameEngine *engine = new GameEngine();
	engine->setWindowSize(-1, -1);
	engine->renameWindow("Spinning Cube");

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

	// The player "cube"
	MetallicOrange* orangeMat = new MetallicOrange();
	CubeObject *cube = new CubeObject();
	cube->translate(glm::vec3(0.0f, 7.5f, 0.0f));
	Script* physics = new BouncePhysics();
	Script* control = new SimpleMovement();
	Script* post = new OutOfBounds();
	cube->setPhysicsTick(physics, true);
	cube->setControlTick(control, true);
	cube->setPostTick(post, true);
	cube->setMaterial(orangeMat, true);
	cube->addTag("player");
	engine->addObject(cube);

	// Make the camera follow the "player"
	Camera* cam = new Camera();
	cam->setFollow(cube, true);
	engine->setCamera(cam, true);

	// Create a particle system
	ParticleSystem* particles = new ParticleSystem("textures/particle.png", 0.0f);
	engine->addObject(particles);

	// Turn on 2 pass rendering
	SecondPassShader* secondPass = new SecondPassShader(1920, 1080);
	engine->setSecondPass(secondPass, true);

	// Add a ground with a slant
	TexturedPlaneObject *ground = new TexturedPlaneObject("textures/ground.png");
	ground->translate(glm::vec3(0.0f, -3.0f, 0.0f));
	ground->scale(glm::vec3(50.0f, 1.0f, 50.0f));
	Quaternion* groundRot = new Quaternion();
	groundRot->euler(-0.5f, 1, 0, 0);
	ground->rotation->hamilton(groundRot);
	ground->getCollider()->trackParentRotation = true;
	ground->getCollider()->recalcTransforms();
	delete groundRot;
	engine->addObject(ground);

	// Info plane
	TexturedPlaneObject *rules = new TexturedPlaneObject("textures/goal.png");
	rules->translate(glm::vec3(0.0f, 0.0f, 0.0f));
	rules->scale(glm::vec3(5.0f, 1.0f, 5.0f));
	Quaternion* rulesRot = new Quaternion();
	rulesRot->euler(3.14f, 0, 0, 1);
	rulesRot->euler(3.14f / 2.0f, 0, 1, 0);
	rules->rotation->hamilton(rulesRot);
	rules->setCollider(nullptr, true);
	delete rulesRot;
	engine->addObject(rules);

	// Create enemies scattered around the world
	const int ENEMY_COUNT = 30;
	std::vector<EnemyObject*> enemies;
	std::vector<Quaternion*> enemiesRot;
	for (int i = 0; i < ENEMY_COUNT; i++) {
		EnemyObject* enemy = new EnemyObject(cube);
		enemy->position = glm::vec3(getRand() * 45.0f, 25.0f, getRand() * 45.0f);
		Script* physics = new BouncePhysics();
		enemy->setPhysicsTick(physics, true);
		enemies.push_back(enemy);
		Quaternion* enemyQuat = new Quaternion();
		enemyQuat->euler(getRand() * 0.05f, getRand(), getRand(), getRand());
		enemiesRot.push_back(enemyQuat);
		engine->addObject(enemy);
	}

	// Render
	while (engine->render()) {
		// Apply the rotation to the player while moving
		if (glm::length(cube->velocity) > 0.02f)
			cube->rotation->hamilton(rotateCube);

		// Apply rotation to enemies
		for (int i = 0; i < ENEMY_COUNT; i++) {
			enemies[i]->rotation->hamilton(enemiesRot[i]);
			// printf("%f %f\n", enemies[i]->velocity.x, enemies[i]->velocity.z);
		}

		// After a while, remove the goal board
		if (rules != nullptr && GameEngine::engineTimer > 180) {
			rules->setDestroy(true);
			rules = nullptr;
		}

		// Player win state, remove enemies
		if (GameEngine::engineTimer > 3600) {
			for (EnemyObject* e : enemies)
				e->setDestroy(true);
			enemies.erase(enemies.begin(), enemies.begin() + enemies.size());
		}

		// Spawn particles, keeping 250 in the world max
		if (particles->size() < 250)
			if (getRand() < 0)
				particles->addParticle(55 * getRand(), 5 * getRand(), 55 * getRand(),
					0.05 * getRand(), 0.05 * getRand(), 0.05 * getRand(), 300);

		// Finish rendering
		engine->postRender();
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
